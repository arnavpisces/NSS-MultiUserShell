all:main client

client:client.c
	gcc -w client.c -o client -lpthread

main:main.c
	gcc main.c -o main -lpthread

clean:
	rm -f main
	rm -f client