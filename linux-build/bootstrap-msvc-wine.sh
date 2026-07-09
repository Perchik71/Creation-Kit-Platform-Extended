#!/usr/bin/env bash
#
# bootstrap-msvc-wine.sh
#
# Installs the msvc-wine toolchain (real Microsoft cl.exe / link.exe / lib.exe / rc.exe
# running under Wine) so the Creation Kit Platform Extended project can be cross-built on
# Linux. See mstorsjo/msvc-wine: https://github.com/mstorsjo/msvc-wine
#
# The toolchain is installed into a customizable prefix (default: ~/.local/share/msvc-wine).
# After a successful run, the CMake toolchain file cmake/msvc-wine-toolchain.cmake will find
# the compiler wrappers automatically.
#
# Usage:   linux-build/bootstrap-msvc-wine.sh [options]
# Help:    linux-build/bootstrap-msvc-wine.sh --help
#
set -euo pipefail

# ----------------------------------------------------------------------------------------
# Constants and defaults
# ----------------------------------------------------------------------------------------
readonly SCRIPT_NAME="${0##*/}"
readonly DEFAULT_PREFIX="${MSVC_WINE_DIR:-$HOME/.local/share/msvc-wine}"
readonly DEFAULT_REPO="https://github.com/mstorsjo/msvc-wine"
readonly TOTAL_STEPS=7
readonly START_TIME="$(date +%s)"

PREFIX="$DEFAULT_PREFIX"
ARCH="x64"
# Default to the stable VS 2022 line (major 17). vsdownload.py itself defaults to
# major 18 (the preview/next Visual Studio), whose MSVC 14.5x STL removed the
# deprecated stdext::checked_array_iterator that Qt 5.12 headers still use.
MAJOR="17"
MSVC_VERSION=""
SDK_VERSION=""
REPO="$DEFAULT_REPO"
BRANCH=""
INSTALL_DEPS=0
FORCE=0
ASSUME_YES=0
VERBOSE=0

# ----------------------------------------------------------------------------------------
# Presentation helpers (color aware, no emoji)
# ----------------------------------------------------------------------------------------
if [[ -t 1 && -z "${NO_COLOR:-}" && "${TERM:-dumb}" != "dumb" ]]; then
    C_RESET=$'\033[0m'; C_BOLD=$'\033[1m'; C_DIM=$'\033[2m'
    C_RED=$'\033[31m'; C_GREEN=$'\033[32m'; C_YELLOW=$'\033[33m'
    C_BLUE=$'\033[34m'; C_CYAN=$'\033[36m'
else
    C_RESET=''; C_BOLD=''; C_DIM=''
    C_RED=''; C_GREEN=''; C_YELLOW=''; C_BLUE=''; C_CYAN=''
fi

_step=0

banner() {
    printf '%s\n' "${C_CYAN}${C_BOLD}"
    printf '+============================================================+\n'
    printf '|                                                            |\n'
    printf '|   CKPE  ::  msvc-wine toolchain bootstrap                   |\n'
    printf '|   real MSVC (cl/link/lib/rc) under Wine, for Linux builds   |\n'
    printf '|                                                            |\n'
    printf '+============================================================+\n'
    printf '%s\n' "${C_RESET}"
}

section() {
    _step=$((_step + 1))
    printf '\n%s[%d/%d] %s%s\n' "${C_BLUE}${C_BOLD}" "$_step" "$TOTAL_STEPS" "$*" "${C_RESET}"
    printf '%s------------------------------------------------------------%s\n' "${C_DIM}" "${C_RESET}"
}

info() { printf '%s  ..%s %s\n' "${C_DIM}" "${C_RESET}" "$*"; }
ok()   { printf '%s  OK%s %s\n' "${C_GREEN}${C_BOLD}" "${C_RESET}" "$*"; }
warn() { printf '%swarn%s %s\n' "${C_YELLOW}${C_BOLD}" "${C_RESET}" "$*" >&2; }
err()  { printf '%s FAIL%s %s\n' "${C_RED}${C_BOLD}" "${C_RESET}" "$*" >&2; }

die() { err "$*"; exit 1; }

run() {
    if (( VERBOSE )); then
        info "\$ $*"
        "$@"
    else
        local _log
        _log="$(mktemp)"
        if ! "$@" >"$_log" 2>&1; then
            err "command failed: $*"
            printf '%s\n' "${C_DIM}"; cat "$_log"; printf '%s\n' "${C_RESET}"
            rm -f "$_log"
            return 1
        fi
        rm -f "$_log"
    fi
}

confirm() {
    (( ASSUME_YES )) && return 0
    local reply
    printf '%s?%s %s [y/N] ' "${C_YELLOW}${C_BOLD}" "${C_RESET}" "$*"
    read -r reply || true
    [[ "$reply" =~ ^[Yy]$ ]]
}

elapsed() {
    local secs=$(( $(date +%s) - START_TIME ))
    printf '%dm%02ds' $((secs / 60)) $((secs % 60))
}

# ----------------------------------------------------------------------------------------
# Usage
# ----------------------------------------------------------------------------------------
usage() {
    cat <<EOF
${C_BOLD}${SCRIPT_NAME}${C_RESET} - install the msvc-wine toolchain for the CKPE Linux cross-build

${C_BOLD}USAGE${C_RESET}
    ${SCRIPT_NAME} [options]

${C_BOLD}OPTIONS${C_RESET}
    --prefix DIR         Install location (default: ${DEFAULT_PREFIX})
                         Also settable via the MSVC_WINE_DIR environment variable.
    --arch ARCH          Target architecture(s): x64 (default), x86, arm, arm64.
                         Comma-separated list allowed, e.g. --arch x64,x86.
    --major VER          Visual Studio major line to pull from (default: 17 =
                         VS 2022). vsdownload's own default (18 = preview/next VS)
                         ships an MSVC 14.5x STL that removed stdext, which Qt 5.12
                         needs; stay on 17 unless you know you want newer.
    --msvc-version VER   Pin a specific MSVC toolset version (default: newest in
                         the chosen major line).
    --sdk-version VER    Pin a specific Windows SDK version (default: latest).
    --repo URL           msvc-wine git repository (default: ${DEFAULT_REPO}).
    --branch NAME        Check out a specific branch/tag of the msvc-wine repo.
    --install-deps       Install missing system packages via the detected package
                         manager (uses sudo). Off by default (detect + instruct only).
    --force              Reinstall even if a toolchain is already present.
    -y, --yes            Assume "yes" for all prompts (non-interactive).
    -v, --verbose        Stream all sub-command output.
    -h, --help           Show this help and exit.

${C_BOLD}EXAMPLES${C_RESET}
    # Default install into ~/.local/share/msvc-wine
    ${SCRIPT_NAME}

    # Custom prefix, auto-install system deps, non-interactive
    ${SCRIPT_NAME} --prefix /opt/msvc-wine --install-deps -y

    # Pin toolset + SDK for reproducible builds
    ${SCRIPT_NAME} --msvc-version 14.42.17.12 --sdk-version 10.0.22621.0

${C_BOLD}AFTER INSTALL${C_RESET}
    cmake --preset msvc-wine
    cmake --build build-linux
EOF
}

# ----------------------------------------------------------------------------------------
# Argument parsing
# ----------------------------------------------------------------------------------------
parse_args() {
    while [[ $# -gt 0 ]]; do
        case "$1" in
            --prefix)        PREFIX="${2:?--prefix needs a value}"; shift 2;;
            --prefix=*)      PREFIX="${1#*=}"; shift;;
            --arch)          ARCH="${2:?--arch needs a value}"; shift 2;;
            --arch=*)        ARCH="${1#*=}"; shift;;
            --major)         MAJOR="${2:?--major needs a value}"; shift 2;;
            --major=*)       MAJOR="${1#*=}"; shift;;
            --msvc-version)  MSVC_VERSION="${2:?--msvc-version needs a value}"; shift 2;;
            --msvc-version=*) MSVC_VERSION="${1#*=}"; shift;;
            --sdk-version)   SDK_VERSION="${2:?--sdk-version needs a value}"; shift 2;;
            --sdk-version=*) SDK_VERSION="${1#*=}"; shift;;
            --repo)          REPO="${2:?--repo needs a value}"; shift 2;;
            --repo=*)        REPO="${1#*=}"; shift;;
            --branch)        BRANCH="${2:?--branch needs a value}"; shift 2;;
            --branch=*)      BRANCH="${1#*=}"; shift;;
            --install-deps)  INSTALL_DEPS=1; shift;;
            --force)         FORCE=1; shift;;
            -y|--yes)        ASSUME_YES=1; shift;;
            -v|--verbose)    VERBOSE=1; shift;;
            -h|--help)       usage; exit 0;;
            *) die "unknown option: $1 (try --help)";;
        esac
    done

    # Expand a leading ~ that survived quoting.
    PREFIX="${PREFIX/#\~/$HOME}"
    # The first arch in the list is the one whose wrappers we probe for.
    PRIMARY_ARCH="${ARCH%%,*}"
}

# ----------------------------------------------------------------------------------------
# System dependency detection
# ----------------------------------------------------------------------------------------
detect_pkg_mgr() {
    if   command -v pacman  >/dev/null 2>&1; then echo pacman
    elif command -v apt-get >/dev/null 2>&1; then echo apt
    elif command -v dnf     >/dev/null 2>&1; then echo dnf
    elif command -v zypper  >/dev/null 2>&1; then echo zypper
    else echo unknown
    fi
}

# Map a logical dependency to the package name for a given package manager.
pkg_for() {
    local mgr="$1" dep="$2"
    case "$mgr:$dep" in
        pacman:wine)        echo wine;;
        pacman:python3)     echo python;;
        pacman:git)         echo git;;
        pacman:msiextract)  echo msitools;;
        apt:wine)           echo wine64;;
        apt:python3)        echo python3;;
        apt:git)            echo git;;
        apt:msiextract)     echo msitools;;
        dnf:wine)           echo wine;;
        dnf:python3)        echo python3;;
        dnf:git)            echo git;;
        dnf:msiextract)     echo msitools;;
        zypper:wine)        echo wine;;
        zypper:python3)     echo python3;;
        zypper:git)         echo git;;
        zypper:msiextract)  echo msitools;;
        *)                  echo "$dep";;
    esac
}

install_cmd() {
    local mgr="$1"; shift
    case "$mgr" in
        pacman) echo "sudo pacman -S --needed $*";;
        apt)    echo "sudo apt-get install -y $*";;
        dnf)    echo "sudo dnf install -y $*";;
        zypper) echo "sudo zypper install -y $*";;
        *)      echo "(install these with your package manager: $*)";;
    esac
}

check_dependencies() {
    section "Checking system prerequisites"

    # wine may be provided as 'wine' or 'wine64'.
    local have_wine=0
    if command -v wine >/dev/null 2>&1 || command -v wine64 >/dev/null 2>&1; then
        have_wine=1
        ok "wine       $(wine --version 2>/dev/null || wine64 --version 2>/dev/null)"
    fi

    local mgr; mgr="$(detect_pkg_mgr)"
    local -a logical_missing=() pkgs_missing=()

    # (logical tool name, whether it is currently satisfied)
    (( have_wine )) || logical_missing+=("wine")
    for tool in python3 git msiextract; do
        if command -v "$tool" >/dev/null 2>&1; then
            ok "$(printf '%-10s' "$tool") $(command -v "$tool")"
        else
            logical_missing+=("$tool")
        fi
    done

    if (( ${#logical_missing[@]} == 0 )); then
        ok "all prerequisites satisfied"
        return 0
    fi

    for dep in "${logical_missing[@]}"; do
        warn "missing: $dep"
        pkgs_missing+=("$(pkg_for "$mgr" "$dep")")
    done

    # De-duplicate package list.
    local -a uniq=(); local p
    for p in "${pkgs_missing[@]}"; do
        [[ " ${uniq[*]:-} " == *" $p "* ]] || uniq+=("$p")
    done

    local cmd; cmd="$(install_cmd "$mgr" "${uniq[@]}")"

    if (( INSTALL_DEPS )); then
        if [[ "$mgr" == unknown ]]; then
            die "could not detect a package manager; install manually: ${uniq[*]}"
        fi
        info "installing missing packages: ${uniq[*]}"
        if confirm "run: $cmd ?"; then
            # shellcheck disable=SC2086
            eval "$cmd" || die "package installation failed"
            ok "system packages installed"
        else
            die "aborted by user"
        fi
    else
        err "missing prerequisites: ${logical_missing[*]}"
        printf '\n     install them with:\n\n        %s%s%s\n\n' "${C_BOLD}" "$cmd" "${C_RESET}"
        printf '     or re-run with %s--install-deps%s to do it automatically.\n' "${C_BOLD}" "${C_RESET}"
        exit 1
    fi
}

# ----------------------------------------------------------------------------------------
# Wine initialization
# ----------------------------------------------------------------------------------------
init_wine() {
    section "Initializing Wine"
    if command -v wineboot >/dev/null 2>&1; then
        info "running wineboot (first run may take a moment)"
        WINEDEBUG="${WINEDEBUG:--all}" run wineboot -i || warn "wineboot returned non-zero (continuing)"
        ok "wine prefix ready"
    else
        warn "wineboot not found; Wine will self-initialize on first use"
    fi
    info "tip: for fast parallel builds keep a persistent server running: ${C_BOLD}wineserver -p${C_RESET}"
}

# ----------------------------------------------------------------------------------------
# Fetch / update the msvc-wine sources
# ----------------------------------------------------------------------------------------
fetch_sources() {
    section "Fetching msvc-wine sources"
    SRC_DIR="$PREFIX/.src"
    mkdir -p "$PREFIX"

    local -a branch_args=()
    [[ -n "$BRANCH" ]] && branch_args=(--branch "$BRANCH")

    if [[ -d "$SRC_DIR/.git" ]]; then
        info "updating existing checkout in $SRC_DIR"
        run git -C "$SRC_DIR" fetch --depth 1 origin || warn "git fetch failed (using cached sources)"
        run git -C "$SRC_DIR" reset --hard "origin/${BRANCH:-HEAD}" 2>/dev/null \
            || run git -C "$SRC_DIR" pull --ff-only || true
    else
        info "cloning $REPO"
        run git clone --depth 1 "${branch_args[@]}" "$REPO" "$SRC_DIR"
    fi

    [[ -f "$SRC_DIR/vsdownload.py" && -f "$SRC_DIR/install.sh" ]] \
        || die "msvc-wine checkout looks incomplete (no vsdownload.py / install.sh)"
    ok "sources ready in $SRC_DIR"
}

# ----------------------------------------------------------------------------------------
# Download + install the toolchain
# ----------------------------------------------------------------------------------------
install_toolchain() {
    section "Downloading and installing MSVC + Windows SDK"
    CL_WRAPPER="$PREFIX/bin/$PRIMARY_ARCH/cl"

    if [[ -x "$CL_WRAPPER" && $FORCE -eq 0 ]]; then
        ok "toolchain already present at $CL_WRAPPER"
        info "re-run with --force to reinstall"
        return 0
    fi

    local -a dl_args=(--dest "$PREFIX" --accept-license)
    # vsdownload uses --arch (comma-separated ok) and --major for the VS line.
    dl_args+=(--arch "$ARCH")
    [[ -n "$MAJOR"        ]] && dl_args+=(--major "$MAJOR")
    [[ -n "$MSVC_VERSION" ]] && dl_args+=(--msvc-version "$MSVC_VERSION")
    [[ -n "$SDK_VERSION"  ]] && dl_args+=(--sdk-version "$SDK_VERSION")

    info "downloading toolchain packages (this can take several minutes)"
    info "major=${MAJOR} arch=${ARCH} msvc=${MSVC_VERSION:-latest} sdk=${SDK_VERSION:-latest}"
    run python3 "$SRC_DIR/vsdownload.py" "${dl_args[@]}"
    ok "packages downloaded"

    info "wrapping toolchain (install.sh)"
    run "$SRC_DIR/install.sh" "$PREFIX"
    ok "toolchain wrapped"
}

# ----------------------------------------------------------------------------------------
# Verify
# ----------------------------------------------------------------------------------------
verify_install() {
    section "Verifying installation"
    CL_WRAPPER="$PREFIX/bin/$PRIMARY_ARCH/cl"
    [[ -x "$CL_WRAPPER" ]] || die "expected compiler wrapper not found: $CL_WRAPPER"
    ok "cl wrapper:   $CL_WRAPPER"
    for tool in link lib rc; do
        if [[ -x "$PREFIX/bin/$PRIMARY_ARCH/$tool" ]]; then
            ok "$(printf '%-4s' "$tool") wrapper: $PREFIX/bin/$PRIMARY_ARCH/$tool"
        else
            warn "$tool wrapper not found (build may still work if provided elsewhere)"
        fi
    done
}

# ----------------------------------------------------------------------------------------
# Summary
# ----------------------------------------------------------------------------------------
print_summary() {
    section "Done"
    printf '%s' "${C_GREEN}${C_BOLD}"
    printf '+------------------------------------------------------------+\n'
    printf '|  msvc-wine toolchain installed successfully                 |\n'
    printf '+------------------------------------------------------------+\n'
    printf '%s' "${C_RESET}"
    printf '\n'
    printf '  %sMSVC_WINE_DIR%s   %s\n' "${C_BOLD}" "${C_RESET}" "$PREFIX"
    printf '  %sarch%s            %s\n' "${C_BOLD}" "${C_RESET}" "$ARCH"
    printf '  %selapsed%s         %s\n' "${C_BOLD}" "${C_RESET}" "$(elapsed)"
    printf '\n  Next steps:\n\n'
    if [[ "$PREFIX" != "$DEFAULT_PREFIX" ]]; then
        printf '    %sexport MSVC_WINE_DIR=%q%s\n' "${C_CYAN}" "$PREFIX" "${C_RESET}"
    fi
    printf '    %scmake --preset msvc-wine%s\n' "${C_CYAN}" "${C_RESET}"
    printf '    %scmake --build build-linux%s\n' "${C_CYAN}" "${C_RESET}"
    printf '\n'
}

# ----------------------------------------------------------------------------------------
# Main
# ----------------------------------------------------------------------------------------
main() {
    parse_args "$@"
    banner

    section "Configuration"
    info "prefix        $PREFIX"
    info "arch          $ARCH"
    info "vs major      ${MAJOR}"
    info "msvc version  ${MSVC_VERSION:-latest}"
    info "sdk version   ${SDK_VERSION:-latest}"
    info "repo          $REPO${BRANCH:+ (branch: $BRANCH)}"
    info "install deps  $([[ $INSTALL_DEPS -eq 1 ]] && echo yes || echo 'no (detect + instruct)')"

    check_dependencies
    init_wine
    fetch_sources
    install_toolchain
    verify_install
    print_summary
}

main "$@"
