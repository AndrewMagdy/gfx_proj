CPP=g++ -std=c++11
CPPFLAGS=-c -Wall

MACCPPLIBS=/lib/libglfw.3.1.dylib
MACCPPFRAMEWORKS=-framework OpenGL GLU
LINUXCPPFRAMEWORKS=-lGL -lGLU

CPPINCLUDES=/include/
CPPFILES=main.cpp loader.cpp mesh.cpp texture.cpp stb_image.c Board.cpp Cell.cpp Location.cpp Player.cpp SmallBoard.cpp

LINUXCURDIR=/home/omar/Downloads/OpenGLObjLoader
MACCURDIR=/Users/andrewmagdy/Downloads/GLMeshLoader-master/



UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)

linux_build:
	$(CPP) $(CPPFILES) -I $(LINUXCURDIR)$(CPPINCLUDES) -L/usr/local/lib -lglfw3 -lrt -lXrandr -lXinerama -lXi -lXcursor -lGL -lm -ldl -lXrender -ldrm -lXdamage -lX11-xcb -lxcb-glx -lxcb-dri2 -lxcb-dri3 -lxcb-present -lxcb-sync -lxshmfence -lXxf86vm -lXfixes -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp -lGLU -o program

endif
ifeq ($(UNAME_S),Darwin)

mac_build:
	$(CPP) $(CPPFILES) -I $(MACCURDIR)$(CPPINCLUDES) -lglfw3   -lfreetype -framework Cocoa -framework sfml-audio -framework OpenGL -framework GLUT -framework IOKit -framework CoreVideo -o program

endif


clean:
	rm -rf *o program
