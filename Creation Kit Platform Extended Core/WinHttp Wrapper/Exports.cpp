//////////////////////////////////////////
/*
* Copyright (c) 2020 Nukem9 <email:Nukem@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

//
// Forward the exports to WINHTTP.dll like normal
//
#pragma comment(linker, "/export:DllCanUnloadNow=C:\\Windows\\System32\\WINHTTP.DllCanUnloadNow")
#pragma comment(linker, "/export:DllGetClassObject=C:\\Windows\\System32\\WINHTTP.DllGetClassObject")
#pragma comment(linker, "/export:Private1=C:\\Windows\\System32\\WINHTTP.Private1")
#pragma comment(linker, "/export:SvchostPushServiceGlobals=C:\\Windows\\System32\\WINHTTP.SvchostPushServiceGlobals")
#pragma comment(linker, "/export:WinHttpAddRequestHeaders=C:\\Windows\\System32\\WINHTTP.WinHttpAddRequestHeaders")
#pragma comment(linker, "/export:WinHttpAutoProxySvcMain=C:\\Windows\\System32\\WINHTTP.WinHttpAutoProxySvcMain")
#pragma comment(linker, "/export:WinHttpCheckPlatform=C:\\Windows\\System32\\WINHTTP.WinHttpCheckPlatform")
#pragma comment(linker, "/export:WinHttpCloseHandle=C:\\Windows\\System32\\WINHTTP.WinHttpCloseHandle")
#pragma comment(linker, "/export:WinHttpConnect=C:\\Windows\\System32\\WINHTTP.WinHttpConnect")
#pragma comment(linker, "/export:WinHttpConnectionDeletePolicyEntries=C:\\Windows\\System32\\WINHTTP.WinHttpConnectionDeletePolicyEntries")
#pragma comment(linker, "/export:WinHttpConnectionDeleteProxyInfo=C:\\Windows\\System32\\WINHTTP.WinHttpConnectionDeleteProxyInfo")
#pragma comment(linker, "/export:WinHttpConnectionFreeNameList=C:\\Windows\\System32\\WINHTTP.WinHttpConnectionFreeNameList")
#pragma comment(linker, "/export:WinHttpConnectionFreeProxyInfo=C:\\Windows\\System32\\WINHTTP.WinHttpConnectionFreeProxyInfo")
#pragma comment(linker, "/export:WinHttpConnectionFreeProxyList=C:\\Windows\\System32\\WINHTTP.WinHttpConnectionFreeProxyList")
#pragma comment(linker, "/export:WinHttpConnectionGetNameList=C:\\Windows\\System32\\WINHTTP.WinHttpConnectionGetNameList")
#pragma comment(linker, "/export:WinHttpConnectionGetProxyInfo=C:\\Windows\\System32\\WINHTTP.WinHttpConnectionGetProxyInfo")
#pragma comment(linker, "/export:WinHttpConnectionGetProxyList=C:\\Windows\\System32\\WINHTTP.WinHttpConnectionGetProxyList")
#pragma comment(linker, "/export:WinHttpConnectionSetPolicyEntries=C:\\Windows\\System32\\WINHTTP.WinHttpConnectionSetPolicyEntries")
#pragma comment(linker, "/export:WinHttpConnectionSetProxyInfo=C:\\Windows\\System32\\WINHTTP.WinHttpConnectionSetProxyInfo")
#pragma comment(linker, "/export:WinHttpConnectionUpdateIfIndexTable=C:\\Windows\\System32\\WINHTTP.WinHttpConnectionUpdateIfIndexTable")
#pragma comment(linker, "/export:WinHttpCrackUrl=C:\\Windows\\System32\\WINHTTP.WinHttpCrackUrl")
#pragma comment(linker, "/export:WinHttpCreateProxyResolver=C:\\Windows\\System32\\WINHTTP.WinHttpCreateProxyResolver")
#pragma comment(linker, "/export:WinHttpCreateUrl=C:\\Windows\\System32\\WINHTTP.WinHttpCreateUrl")
#pragma comment(linker, "/export:WinHttpDetectAutoProxyConfigUrl=C:\\Windows\\System32\\WINHTTP.WinHttpDetectAutoProxyConfigUrl")
#pragma comment(linker, "/export:WinHttpFreeProxyResult=C:\\Windows\\System32\\WINHTTP.WinHttpFreeProxyResult")
#pragma comment(linker, "/export:WinHttpFreeProxyResultEx=C:\\Windows\\System32\\WINHTTP.WinHttpFreeProxyResultEx")
#pragma comment(linker, "/export:WinHttpFreeProxySettings=C:\\Windows\\System32\\WINHTTP.WinHttpFreeProxySettings")
#pragma comment(linker, "/export:WinHttpGetDefaultProxyConfiguration=C:\\Windows\\System32\\WINHTTP.WinHttpGetDefaultProxyConfiguration")
#pragma comment(linker, "/export:WinHttpGetIEProxyConfigForCurrentUser=C:\\Windows\\System32\\WINHTTP.WinHttpGetIEProxyConfigForCurrentUser")
#pragma comment(linker, "/export:WinHttpGetProxyForUrl=C:\\Windows\\System32\\WINHTTP.WinHttpGetProxyForUrl")
#pragma comment(linker, "/export:WinHttpGetProxyForUrlEx=C:\\Windows\\System32\\WINHTTP.WinHttpGetProxyForUrlEx")
#pragma comment(linker, "/export:WinHttpGetProxyForUrlEx2=C:\\Windows\\System32\\WINHTTP.WinHttpGetProxyForUrlEx2")
#pragma comment(linker, "/export:WinHttpGetProxyForUrlHvsi=C:\\Windows\\System32\\WINHTTP.WinHttpGetProxyForUrlHvsi")
#pragma comment(linker, "/export:WinHttpGetProxyResult=C:\\Windows\\System32\\WINHTTP.WinHttpGetProxyResult")
#pragma comment(linker, "/export:WinHttpGetProxyResultEx=C:\\Windows\\System32\\WINHTTP.WinHttpGetProxyResultEx")
#pragma comment(linker, "/export:WinHttpGetProxySettingsVersion=C:\\Windows\\System32\\WINHTTP.WinHttpGetProxySettingsVersion")
#pragma comment(linker, "/export:WinHttpGetTunnelSocket=C:\\Windows\\System32\\WINHTTP.WinHttpGetTunnelSocket")
#pragma comment(linker, "/export:WinHttpOpen=C:\\Windows\\System32\\WINHTTP.WinHttpOpen")
#pragma comment(linker, "/export:WinHttpOpenRequest=C:\\Windows\\System32\\WINHTTP.WinHttpOpenRequest")
#pragma comment(linker, "/export:WinHttpProbeConnectivity=C:\\Windows\\System32\\WINHTTP.WinHttpProbeConnectivity")
#pragma comment(linker, "/export:WinHttpQueryAuthSchemes=C:\\Windows\\System32\\WINHTTP.WinHttpQueryAuthSchemes")
#pragma comment(linker, "/export:WinHttpQueryDataAvailable=C:\\Windows\\System32\\WINHTTP.WinHttpQueryDataAvailable")
#pragma comment(linker, "/export:WinHttpQueryHeaders=C:\\Windows\\System32\\WINHTTP.WinHttpQueryHeaders")
#pragma comment(linker, "/export:WinHttpQueryOption=C:\\Windows\\System32\\WINHTTP.WinHttpQueryOption")
#pragma comment(linker, "/export:WinHttpReadData=C:\\Windows\\System32\\WINHTTP.WinHttpReadData")
#pragma comment(linker, "/export:WinHttpReadProxySettings=C:\\Windows\\System32\\WINHTTP.WinHttpReadProxySettings")
#pragma comment(linker, "/export:WinHttpReadProxySettingsHvsi=C:\\Windows\\System32\\WINHTTP.WinHttpReadProxySettingsHvsi")
#pragma comment(linker, "/export:WinHttpReceiveResponse=C:\\Windows\\System32\\WINHTTP.WinHttpReceiveResponse")
#pragma comment(linker, "/export:WinHttpResetAutoProxy=C:\\Windows\\System32\\WINHTTP.WinHttpResetAutoProxy")
#pragma comment(linker, "/export:WinHttpSaveProxyCredentials=C:\\Windows\\System32\\WINHTTP.WinHttpSaveProxyCredentials")
#pragma comment(linker, "/export:WinHttpSendRequest=C:\\Windows\\System32\\WINHTTP.WinHttpSendRequest")
#pragma comment(linker, "/export:WinHttpSetCredentials=C:\\Windows\\System32\\WINHTTP.WinHttpSetCredentials")
#pragma comment(linker, "/export:WinHttpSetDefaultProxyConfiguration=C:\\Windows\\System32\\WINHTTP.WinHttpSetDefaultProxyConfiguration")
#pragma comment(linker, "/export:WinHttpSetOption=C:\\Windows\\System32\\WINHTTP.WinHttpSetOption")
#pragma comment(linker, "/export:WinHttpSetStatusCallback=C:\\Windows\\System32\\WINHTTP.WinHttpSetStatusCallback")
#pragma comment(linker, "/export:WinHttpSetTimeouts=C:\\Windows\\System32\\WINHTTP.WinHttpSetTimeouts")
#pragma comment(linker, "/export:WinHttpTimeFromSystemTime=C:\\Windows\\System32\\WINHTTP.WinHttpTimeFromSystemTime")
#pragma comment(linker, "/export:WinHttpTimeToSystemTime=C:\\Windows\\System32\\WINHTTP.WinHttpTimeToSystemTime")
#pragma comment(linker, "/export:WinHttpWebSocketClose=C:\\Windows\\System32\\WINHTTP.WinHttpWebSocketClose")
#pragma comment(linker, "/export:WinHttpWebSocketCompleteUpgrade=C:\\Windows\\System32\\WINHTTP.WinHttpWebSocketCompleteUpgrade")
#pragma comment(linker, "/export:WinHttpWebSocketQueryCloseStatus=C:\\Windows\\System32\\WINHTTP.WinHttpWebSocketQueryCloseStatus")
#pragma comment(linker, "/export:WinHttpWebSocketReceive=C:\\Windows\\System32\\WINHTTP.WinHttpWebSocketReceive")
#pragma comment(linker, "/export:WinHttpWebSocketSend=C:\\Windows\\System32\\WINHTTP.WinHttpWebSocketSend")
#pragma comment(linker, "/export:WinHttpWebSocketShutdown=C:\\Windows\\System32\\WINHTTP.WinHttpWebSocketShutdown")
#pragma comment(linker, "/export:WinHttpWriteData=C:\\Windows\\System32\\WINHTTP.WinHttpWriteData")
#pragma comment(linker, "/export:WinHttpWriteProxySettings=C:\\Windows\\System32\\WINHTTP.WinHttpWriteProxySettings")