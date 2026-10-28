# Промпт:
# 
# Необходимо написать на `Python` скрипт:
# - Сканирует подкаталоги по определённому пути и находит в них файлы с расширением `.h`.
# - В файлах `.h` (являются `C/C++` заголовочными файлами) найти объявления классов и требуется получить имя класса и всех унаследованных классов, информацию собрать в один контейнер.
# - Необходимо определить перечень классов, унаследованных от заданного пользователем: `A->B->C`, если пользователь указал найти от `A` он должен указать только `С`.
# - Учесть неявное наследование классов: `A->B`, где далее в другом `.h` `B->C`, если пользователь указал найти от `A` он должен указать только `С` (Если `B` был найден ранее и добавлен в список, он должен быть удалён из списка найденных, однако, стоит сохранить информацию, что `B` является классом для включения найденных классов от него самого).
# - Перечень найденных классов передать в `stdout`.
# 
# Пример:
# 
# class TESForm : public BaseFormComponent
# {
# 
# class TESObject :
# 				public TESForm   // 000
# {
# 
# class TESObjectARMA :
# 				public TESObject,           // 000
# 				public TESRaceForm,         // 030
# 				public BGSBipedObjectForm   // 040
# {
#
# Следующий промпт:
#
# Не учитывает более сложные наследования:
# 
# TESForm->TESObject->TESBoundObject
# 
# class TESObjectMISC :
#				public TESBoundObject,             // 000
#				public TESFullName,                // 090
#				public BGSModelMaterialSwap,       // 0A8
#				public TESIcon,                    // 0F8
#				public TESValueForm,               // 120
#				public TESWeightForm,              // 130
#				public BGSDestructibleObjectForm,  // 140
#				public BGSMessageIcon,             // 150
#				public BGSPickupPutdownSounds,     // 180
#				public BGSKeywordForm,             // 198
#				public BGSFeaturedItemMessage      // 1B8
# {

import os
import re
import sys
from collections import defaultdict, deque
from pathlib import Path

# Регулярка для объявления класса.
# Учитывает случаи, когда ":" и список базовых классов могут быть на следующих строках,
# а также когда в списке базовых классов есть комментарии // ...
# Мы сначала находим блок "class Name ... {", а затем отдельно парсим список базовых классов.
CLASS_DECL_RE = re.compile(
    r'\bclass\s+([A-Za-z_][A-Za-z0-9_]*)\s*(?::\s*)?',
    re.MULTILINE
)

def extract_base_classes_from_block(code_block: str):
    """
    Извлекает имена базовых классов из фрагмента кода, содержащего объявление класса.
    Поддерживает:
      - public/protected/private
      - многострочность
      - комментарии // после базовых классов
    Возвращает список имён базовых классов (без спецификаторов доступа).
    """
    # Сначала убираем комментарии // до конца строки
    no_comments = re.sub(r'//.*$', '', code_block, flags=re.MULTILINE)

    # Ищем часть после ':' до '{'
    match = re.search(r':\s*([^\{]+)\{', no_comments)
    if not match:
        return []

    bases_part = match.group(1)

    # Разбиваем по запятым, но аккуратно: не хотим разбивать внутри шаблонов (хотя это сложно без полноценного парсера)
    # Для типичных случаев (как у тебя) этого достаточно.
    candidates = [p.strip() for p in bases_part.split(',')]

    bases = []
    for c in candidates:
        if not c:
            continue
        # Убираем спецификаторы доступа
        m = re.match(r'(?:public|protected|private)\s+(.+)', c)
        name = m.group(1).strip() if m else c.strip()
        # Оставляем только имя класса (простейшая очистка от шаблонов и прочего)
        # Если есть '<', то берём часть до '<' как имя (это эвристика)
        if '<' in name:
            name = name.split('<')[0].strip()
        if name:
            bases.append(name)
    return bases


def scan_headers(root_path: str):
    headers = []
    root = Path(root_path)
    for path in root.rglob("*.h"):
        if path.is_file():
            try:
                content = path.read_text(encoding="utf-8", errors="ignore")
                headers.append((path, content))
            except Exception:
                continue
    return headers


def parse_classes_from_content(content: str):
    """
    Находит все объявления классов в тексте и извлекает их базовые классы.
    Возвращает список кортежей (class_name, [base_class_names]).
    """
    results = []
    # Находим позиции каждого 'class Name'
    for m in CLASS_DECL_RE.finditer(content):
        start_pos = m.start()
        class_name = m.group(1)

        # Ищем открывающую '{' после этого объявления.
        # Это нужно, чтобы выделить блок, где может быть список базовых классов.
        brace_pos = content.find('{', start_pos)
        if brace_pos == -1:
            continue  # некорректное объявление, пропускаем

        # Выделяем фрагмент от начала объявления до '{' (включительно), чтобы вытащить базовые классы
        block = content[start_pos:brace_pos + 1]

        bases = extract_base_classes_from_block(block)
        results.append((class_name, bases))

    return results


def build_inheritance_graph(headers):
    direct_parents = defaultdict(list)  # child -> [parents]
    all_children = defaultdict(list)    # parent -> [children]
    all_classes = set()

    for filepath, content in headers:
        for class_name, bases in parse_classes_from_content(content):
            all_classes.add(class_name)
            for base in bases:
                direct_parents[class_name].append(base)
                all_children[base].append(class_name)

    return direct_parents, all_children, all_classes


def find_transitive_descendants(root_class, all_children):
    """BFS: все транзитивные потомки root_class."""
    visited = set()
    queue = deque()

    if root_class in all_children:
        queue.extend(all_children[root_class])
        visited.update(all_children[root_class])

    while queue:
        current = queue.popleft()
        for child in all_children.get(current, []):
            if child not in visited:
                visited.add(child)
                queue.append(child)

    return visited


def filter_intermediate_classes(descendants, direct_parents):
    """
    Удаляет из списка descendants все классы, которые являются родителями
    для других классов, также находящихся в descendants.
    Остаются только «листовые» классы относительно этого подмножества потомков.
    """
    # Построим локальный all_children из direct_parents
    temp_all_children = defaultdict(list)
    for child, parents in direct_parents.items():
        for p in parents:
            temp_all_children[p].append(child)

    final_list = [
        cls for cls in descendants
        if not any(child in descendants for child in temp_all_children.get(cls, []))
    ]
    return final_list


def main():
    if len(sys.argv) < 3:
        print("Использование: python find_transitive_classes.py <путь_к_корневой_папке> <имя_базового_класса>", file=sys.stderr)
        sys.exit(1)

    root_path = sys.argv[1]
    base_class = sys.argv[2]

    headers = scan_headers(root_path)
    if not headers:
        print(f"Не найдено .h файлов в '{root_path}'", file=sys.stderr)
        sys.exit(0)

    direct_parents, all_children, all_classes = build_inheritance_graph(headers)

    descendants = find_transitive_descendants(base_class, all_children)
    final_classes = filter_intermediate_classes(descendants, direct_parents)

    # Вывод в stdout
    for cls in sorted(final_classes):
        print(cls)


if __name__ == "__main__":
    main()
