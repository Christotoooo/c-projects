#!/bin/bash
for VARIABLE_NAME in $(ls data);
	do
	mv data/$VARIABLE_NAME query/
	 ./movie_recommender query/$VARIABLE_NAME data/*
	mv query/$VARIABLE_NAME data
done