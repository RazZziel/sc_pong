
# TargetBegin: name: sc_pong, type: executable
ADD_EXECUTABLE (sc_pong CBall.cpp CPlayer.cpp error.cpp main.cpp CGame.cpp CFramerate.cpp menus.cpp CAnimation.cpp CCtrlKeyboard.cpp CCtrlJoystick.cpp CCtrlNet.cpp CControl.cpp audio.cpp CPhysics.cpp CVector.cpp CGraphics.cpp widgets.cpp scfont.cpp )
TARGET_LINK_LIBRARIES (sc_pong -lopenal)
TARGET_LINK_LIBRARIES (sc_pong -lalut)
TARGET_LINK_LIBRARIES (sc_pong -lSDL_gfx)
TARGET_LINK_LIBRARIES (sc_pong -lSDL_image)
TARGET_LINK_LIBRARIES (sc_pong -lSDL_mixer)
TARGET_LINK_LIBRARIES (sc_pong -lSDL_ttf)
# TargetEnd
