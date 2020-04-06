// ITP 365 Spring 2019
// HW7 - IMDB Graph
// Name: Mara Pleasure
// Email: mpleasur@usc.edu
// Platform: Mac
// IDMBData.cpp - Implements IMDBData interface

#include "IMDBData.h"
#include <fstream>
#include <iostream>
#include <queue>

// Do for Part 1
// Function: Default Constructor
// Purpose: Loads in the actor/movie file specified,
// and populates the data into the appropriate hash maps.
// Also constructs the graph with actor relationships.
// Input: Name of file to load (by reference)
// Returns: Nothing
IMDBData::IMDBData(const std::string& fileName)
{
    
    //file open procedures and variable intilization
    std::ifstream fileInput(fileName);
    std::string line;
    std::string name;
    std::vector<std::string> movies;
    
    //check if file is open
    if(fileInput.is_open()){
        while(!fileInput.eof()){
            getline(fileInput, line);
            //checks if the line[0] has the special character to see if it is a movie
            if (line[0] != '|'){
                //need to clear the vector
                movies.clear();
                name = line;
            }
            //if it does have a character, it is a movie
            if (line[0] == '|'){
                movies.push_back(line.substr(1));
            }
                //for (int i = 0; i<movies.size(); i++){
                 //   std::string cutString;
                 //   //std::cout << "Movie name: " << movies[i] << std::endl;
                    
                  //  }
            
            //add to map
            mActorsToMoviesMap[name] = movies;
            //reverse map
            reverseMap(name, movies);
            }
        
    }
    else {
        //error message
        std::cout << "file didn't open!" << std::endl;
    }
    
}


// Do for Part 1
// Function: reverseMap
// Purpose: This is a helper function that is used to generate
// the reverse map (movies to actors)
// Input: Name of actor (by reference), vector of their movies (by reference)
// Returns: Nothing
void IMDBData::reverseMap(const std::string& actorName, const std::vector<std::string>& movies)
{
	//1. if mMovieToActors map already contanis the movie in question, add actorName to that movie's associated vector
    //2. otherwise you need to add an entry in mMovieToActorsMap that associates the movie with a vector that, for now only contains actorName
    
    //loop through movies
    for (int i=0; i<movies.size(); i++){
        //if the movie is in the map already....
        if (mMoviesToActorsMap.find(movies[i]) != mActorsToMoviesMap.end()){
            mMoviesToActorsMap[movies[i]].push_back(actorName);
        }
        else{
            //if the movie is not in the map, then add it
            std::vector<std::string> newVector;
            newVector.push_back(actorName);
            mMoviesToActorsMap[movies[i]] = newVector;
        }
    }

    
}

// Do for Part 1
// Function: getMoviesFromActor
// Purpose: Given an actor's name, returns a vector containing their movies
// Input: Name of actor (by reference)
// Returns: Vector of movies (by reference)
std::vector<std::string>& IMDBData::getMoviesFromActor(const std::string& actorName)
{
    //first check to make sure the actor is in the map
    if(mActorsToMoviesMap.find(actorName) != mActorsToMoviesMap.end()){
        //return the movies for the actor if they are
        return mActorsToMoviesMap[actorName];
        
    }
    else{
        //else return empty
        return sEmptyVector;
    }
	
}

// Do for Part 1
// Function: getActorsFromMovie
// Purpose: Given a movie's name, returns the name of its actors
// Input: Name of movie (by reference)
// Returns: Vector of actors (by reference)
std::vector<std::string>& IMDBData::getActorsFromMovie(const std::string& movieName)
{
   
    //first check to make sure the movie is in the map
    if (mMoviesToActorsMap.find(movieName) != mMoviesToActorsMap.end()){
        //then get list of actors
        return mMoviesToActorsMap[movieName];
        
    }
    else{
        //else return empty vector
        return sEmptyVector;
    }
	
}

// Do for Part 2
// Function: findRelationship
// Purpose: Tries to find a relationship between two actors, using a BFS
// and outputs (to cout) the result of the search.
// Input: Name of two actors to check for a relationship
// Returns: Nothing
void IMDBData::findRelationship(const std::string& fromActor, const std::string& toActor)
{
	//IMDBgraph has a few different member functions, including
    //1. containsActor which tells you whether or not an actor is in the graph
    //2. getActorNode, which returns a pointer to an actor node
    
    //the actor node struct has a adjancency list (vector of edges), a pointer and a visited bool
    //the edge node struct has the name of the movie, as well as the pointer to the other actor
    
    //First check to see if the actors are in the graph - if not cout error
    ActorNode* currentNode;
    bool pathFound = false;

    //first check that the actors are both there
    if (mGraph.containsActor(fromActor)){
        if (mGraph.containsActor(toActor)){
            //then make a queue
            std::queue<ActorNode*> bfsQue;
            bfsQue.push(mGraph.getActorNode(fromActor)); //use get actor node function from graph
            //currentNode = bfsQue.front();
            while (bfsQue.size() != 0){
                //set current node to front
                currentNode = bfsQue.front();
                //pop first item - deque it
                bfsQue.pop();
                if (currentNode->mName == toActor){
                    //if a pathfound set to tru
                    pathFound = true;
                }
                //check if current node has been visited
                else if(currentNode->mIsVisited == false){
                    //if not set to true
                    currentNode->mIsVisited = true;
                    //check edges
                    for (const auto& i : currentNode->mEdges){
                        //if the edges havent been visited
                        if(i.mOtherActor->mIsVisited == false){
                            //add to queue
                            bfsQue.push(i.mOtherActor);
                            //if the path is 0
                            if (i.mOtherActor->mPath.size() == 0){
                                //set paths equal
                                i.mOtherActor->mPath = currentNode->mPath;
                                //make new path pair
                                PathPair newPath(i.mMovieName,i.mOtherActor->mName);
                                //push back new path pair
                                i.mOtherActor->mPath.push_back(newPath);
                            }
                            
                        }
                    }
                }
            
            }
            if (pathFound == true){
                //make an actor node to use
                ActorNode* toActorNode;
                toActorNode = mGraph.getActorNode(toActor);
                //to get hops number
                int pathNum = 0;
                pathNum = toActorNode->mPath.size();
                
                //starting
                std::cout<< "Path Found (" << pathNum <<")" << std::endl;
                std::cout << fromActor << " was in... "<< std::endl;
                //range based loop to get movie names and actor names
                for (const auto& j : toActorNode->mPath){
                    std::cout<<j.getMovieName() << " with " << j.getActorName() << " who was in..." <<std::endl;
                    
                }
            }
            //else no path found
            if (pathFound == false){
                std::cout<< "Path not found" << std::endl;
            }

        }
        

    }
    else{
        std::cout << "Actor is not found" << std::endl;
    }
    
   
    
    //Next, you'll want to implement BFS as outlined in lecture, the adjancency list in this case is a vector of edges, not a vector of actorNode pointers. But you can find the connecting actorNode by accessing the mOtherActor member of the edge
    
    //dont forget, pointers use -> to access members, but references use . to access members
    
    //make sure you set each actor's node to mIsVisited variable as you visit it
    
    //BFS should finish in one of two cases:
    //     1. the queue becomes empty (failed to find path)
    //     2. the second actor was found
    
    //once BFS finishes, output "found a path" or "didn't find path"

	// LEAVE THIS AT THE END OF THE FUNCTION
	mGraph.clearVisited();
}

// For Part 2
// Function: createGraph
// Purpose: This helper function constructs the IMDBGraph from the movie to actors map
// Input: None
// Returns: Nothing
void IMDBData::createGraph()
{
	// DO NOT EDIT THIS FUNCTION
	// For every movie in the actors map...
	for (auto& p : mMoviesToActorsMap)
	{
		const std::string& movieName = p.first;
		// Get the actors for this movie
		const std::vector<std::string>& actors = mMoviesToActorsMap[movieName];

		// Make n^2 edges between these actors
		for (size_t i = 0; i < actors.size(); i++)
		{
			ActorNode* firstActor = mGraph.getActorNode(actors[i]);
			for (size_t j = i + 1; j < actors.size(); j++)
			{
				ActorNode* secondActor = mGraph.getActorNode(actors[j]);
				mGraph.createActorEdge(firstActor, secondActor, movieName);
			}
		}
	}
}

// Leave here! Do not edit!!! Use in part 1
// DO NOT REMOVE THIS LINE
std::vector<std::string> IMDBData::sEmptyVector;
