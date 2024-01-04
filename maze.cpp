#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

// Represents a vertex in the graph
struct Vertex 
{
    int vertex; // Used to identify individual vertices
    vector<int> nextMoves;
    string color;
};

// Represents the graph - Adjacency List used
class AdjList 
{
    public:

        // Add a vertex to the graph (represents any given arrow)
        void addVertex(int vertex, const string& color = "") 
        {
            vertices.emplace_back(Vertex{vertex, {}, color});
        }

        // Add an edge between two adjacent vertices (between current move and potential next moves)
        void addEdge(int start, int end) 
        {
            vertices[start].nextMoves.push_back(end);
        }

        // DFS algorithm to find a path from start to goal
        vector<int> dfs(int start, int goal) 
        {
            stack<pair<int, vector<int>>> stack;
            stack.push({start, {start}});
            vector<int> visited; // visited vertices

            while (!stack.empty()) 
            {
                auto [vertex, path] = stack.top();
                stack.pop();

                if (vertex == goal) 
                {
                    return path; // return path to goal
                }

                if (find(visited.begin(), visited.end(), vertex) == visited.end()) 
                {
                    visited.push_back(vertex);

                    for (int next : vertices[vertex].nextMoves) 
                    {
                        auto addedPath = path;
                        addedPath.push_back(next);
                        stack.push({next, addedPath});
                    }
                }
            }

            return {}; // Return an empty path if no path is found
        }

        // Print the path to the output file
        void findPath(const vector<int>& path, const vector<vector<string>>& directionsList, int cols) 
        {
            ofstream output("output.txt");

            for (size_t i = 0; i < path.size() - 1; ++i) 
            {
                int currVertex = path[i];
                int nextVertex = path[i + 1];

                // Calculate number of steps in each direction
                int numOfSteps = max(abs(nextVertex / cols - currVertex / cols), abs(nextVertex % cols - currVertex % cols));
                const string& direction = directionsList[currVertex / cols][currVertex % cols];

                // Write to output file in expected format
                output << numOfSteps << direction << ' ';
            }

            output.close();
        }

        vector<Vertex> vertices;
};

// Add edges in specified direction
void addEdgesInDirection(int startRow, int startCol, int rowChange, int colChange, const vector<vector<string>>& directionsList, const vector<vector<int>>& vertexList, AdjList& adjList, int numRows, int numCols)
{
    int nextRow = startRow + rowChange; // Calculate the next row
    int nextCol = startCol + colChange; // Calculate the next column

    // Iterate until out of bounds
    while (nextRow >= 0 && nextRow < numRows && nextCol >= 0 && nextCol < numCols) 
    {
        // Check that colors are opposites
        if (adjList.vertices[vertexList[startRow][startCol]].color != adjList.vertices[vertexList[nextRow][nextCol]].color) 
        {
            adjList.addEdge(vertexList[startRow][startCol], vertexList[nextRow][nextCol]);
        }
        nextRow += rowChange;
        nextCol += colChange;
    }
}

int main() 
{
    string inputFileName = "apollodiana.txt";

    // Initialize adjacency list
    AdjList adjList;

    // Read input from the file
    ifstream inputFile(inputFileName);

    vector<vector<string>> directionsList;
    vector<vector<int>> vertexList;

    // Read number of rows and cols from the first line
    int numRows, numCols;
    inputFile >> numRows >> numCols;

    // Add nodes to the graph
    for (int i = 0; i < numRows * numCols; ++i) 
    {
        adjList.addVertex(i);
    }

    // Make edges in the graph based on directions
    for (int row = 0; row < numRows; ++row) 
    {
        vector<string> directionsRow;
        vector<int> vertexRow;

        for (int col = 0; col < numCols; ++col) 
        {
            string arrow;
            inputFile >> arrow;

            // Check that current arrow is not goal
            if (arrow == "O") 
            {
                vertexRow.push_back(row * numCols + col);
                directionsRow.push_back("");
            } 
            else 
            {
                // Save color and direction of arrow based on input format
                string color = arrow.substr(0, 1);
                string direction = arrow.substr(2);

                adjList.vertices[row * numCols + col].color = color;
                vertexRow.push_back(row * numCols + col);
                directionsRow.push_back(direction);
            }
        }

        directionsList.push_back(directionsRow);
        vertexList.push_back(vertexRow);
    }

    // Make edges based on directions
    for (int row = 0; row < numRows; ++row) 
    {
        for (int col = 0; col < numCols; ++col) 
        {
            if (directionsList[row][col] == "N") 
            {
                // Move up one row
                addEdgesInDirection(row, col, -1, 0, directionsList, vertexList, adjList, numRows, numCols);
            }
            else if (directionsList[row][col] == "S") 
            {
                // Move down one row
                addEdgesInDirection(row, col, 1, 0, directionsList, vertexList, adjList, numRows, numCols);
            }
            else if (directionsList[row][col] == "E")  
            {
                // Move right one column
                addEdgesInDirection(row, col, 0, 1, directionsList, vertexList, adjList, numRows, numCols);
            }
            else if (directionsList[row][col] == "W")
            {
                // Move left one column
                addEdgesInDirection(row, col, 0, -1, directionsList, vertexList, adjList, numRows, numCols);
            }
            else if (directionsList[row][col] == "NE") 
            {
                // Move up one row and right one column
                addEdgesInDirection(row, col, -1, 1, directionsList, vertexList, adjList, numRows, numCols);
            }
            else if(directionsList[row][col] == "SE")
            {
                // Move down one row and right one column
                addEdgesInDirection(row, col, 1, 1, directionsList, vertexList, adjList, numRows, numCols);
            }
            else if(directionsList[row][col] == "SW")
            {
                // Move down one row and left one column
                addEdgesInDirection(row, col, 1, -1, directionsList, vertexList, adjList, numRows, numCols);
            }
            else if(directionsList[row][col] == "NW")
            {
                // Move up one row and left one column
                addEdgesInDirection(row, col, -1, -1, directionsList, vertexList, adjList, numRows, numCols);
            }
        }
    }

    // Perform DFS search
    int goal = numRows * numCols - 1;
    vector<int> verticesInPath = adjList.dfs(0, goal); // Store vertices visited in path to goal

    // Print the path to the output file
    adjList.findPath(verticesInPath, directionsList, numCols);

    return 0;
}