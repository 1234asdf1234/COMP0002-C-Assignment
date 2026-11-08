maze_cmd.o: maze_cmd.c
	gcc -c maze_cmd.c -o build/maze_cmd.o

bot_cmd.o: bot_cmd.c
	gcc -c bot_cmd.c -o build/bot_cmd.o

search.o: search.c
	gcc -c search.c -o build/search.o

main.o: main.c
	gcc -c main.c -o build/main.o

main: maze_cmd.o bot_cmd.o search.o main.o
	gcc build/maze_cmd.o build/bot_cmd.o build/search.o build/main.o -o build/maze_cmd



