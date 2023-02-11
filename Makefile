BUILD_DIR = ./build
EXEC_FILE = gravi_sim.exec
CFLAGS = -pedantic -Wall -g -Wextra -Werror
CPP_VERSION = -std=c++11
SFML_FLAGS = -framework sfml-graphics -framework sfml-window -framework sfml-system

ra: all run	

all:
	g++ -c ${CPP_VERSION} ${CFLAGS} Main.cpp -o ${BUILD_DIR}/main.o
	g++ -c ${CPP_VERSION} ${CFLAGS} Body.cpp -o ${BUILD_DIR}/body.o
	g++ -c ${CPP_VERSION} ${CFLAGS} Physics.cpp -o ${BUILD_DIR}/physics.o
	g++ ${BUILD_DIR}/main.o ${BUILD_DIR}/body.o ${BUILD_DIR}/physics.o -o ${BUILD_DIR}/${EXEC_FILE} -lsfml-graphics -lsfml-window -lsfml-system

$(shell mkdir -p build)

run:
	${BUILD_DIR}/${EXEC_FILE}
