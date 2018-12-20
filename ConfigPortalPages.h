#pragma once

#define CP_NetworkFragment "<option value=\"{ssid}\" {selected}>{ssid} ({quality}%)</option>"
#define CP_RootBody "<form action=\"/save\" method=\"post\"><div><span>Network name</span><select name=\"ssid\">{networks}</select></div>" \
                    "<div><span>Password</span><input type=\"password\" name=\"password\"></div>" \
					"<input type=\"submit\" value=\"Save\"></form>"
#define CP_PageHeader "<!DOCTYPE html><html><head><meta charset=\"utf-8\" />"\
					"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"\
					"<title>{title}</title>"\
					"<link href=\"/style\" rel=\"stylesheet\" />"\					"</head><body><div class=\"p\"><header><h1>{title}</h1></header>"
#define CP_PageFooter "</div></body></html>"
#define CP_Stylesheet "body, h1{margin:0;} h1, form div span{padding:5px;} h1 {background: #44546A;color:white;}"\
                      "select, input {width:90%;display:block;margin:0 auto 10px auto;} select{height:30px;}"
#define CP_SaveSuccessBody "<div>Save successful, the device will reset in a few seconds</div>"
#define CP_SaveFailedBody "<div>Can't connect to WiFi, <a href=\"/\">click here to return to the confguration page</a></div>"
#define CP_NotFoundBody "<div>Oops, can't find this page, <a href=\"/\">click here to return to the confguration page</a></div>"
