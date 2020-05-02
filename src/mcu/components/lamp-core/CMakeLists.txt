set (COMPONENT_ADD_INCLUDEDIRS
    "../../core"
    "../../core/handler"
    "../../core/pal"
    "../../core/pal/handler"
    "../../core/pal/platform"
	)

set (COMPONENT_SRCS
	"../../core/Lamp.cpp"
    "../../core/LedsManager.cpp"
    "../../core/UpgradeManager.cpp"
    "../../core/handler/HttpHandler.cpp"
    "../../core/handler/IotHandler.cpp"
    "../../core/handler/IrHandler.cpp"
    "../../core/handler/WiFiHandler.cpp"
	)

register_component()