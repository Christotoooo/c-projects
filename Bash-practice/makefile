
all: movie_recommender

movie_recommender: distances.o preferences.o src/movie_recommender.c
	gcc -Iinclude distances.o preferences.o src/movie_recommender.c -o movie_recommender
	
distances.o: src/distances.c include/distances.h
	gcc -Iinclude -c src/distances.c
	
preferences.o: include/preferences.h src/preferences.c
	gcc -Iinclude -c src/preferences.c

clean: 
	rm -rf movie_recommender distances.o preferences.o