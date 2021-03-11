/*******************************************************************************
 * Name    : waterjugpuzzle.cpp
 * Author  : Jose de la Cruz, Breanna Shinn
 * Version : 1.0
 * Date    : 10/18/2020
 * Description : Waterjug puzzle solver.
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/
#include <iostream>
#include <sstream>
#include <queue>
#include <stack>

using namespace std;

// Struct to represent state of water in the jugs.
struct State {
    int a, b, c;
    string directions;
    State *parent;
    
    State(int _a, int _b, int _c, string _directions) : 
        a{_a}, b{_b}, c{_c}, directions{_directions}, parent{nullptr} { }
    
    // String representation of state in tuple form.
    string to_string() {
        ostringstream oss;
        oss << "(" << a << ", " << b << ", " << c << ")";
        return oss.str();
    }
};

bool check_input(int argc, char * const argv[]){
    if(argc != 7){
        cout << "Usage: ./waterjugpuzzle <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>";
        return false;
    }

    for (size_t i = 1; i < 4; i++) // check capacities
    {
        istringstream iss;
        int x;
        iss.str(argv[i]);
        
        if (!(iss >> x) || (x <= 0)){
            string letter;
            switch (i)
            {
            case 1:{
                letter = "A";
                break;
            }
            case 2:{
                letter = "B";
                break;
            }
            case 3:{
                letter = "C";
                break;
            }
            default:
                break;
            }

            cout << "Error: Invalid capacity '" << argv[i] << "' for jug " << letter << ".";
            return false;
        }
    }

    for (size_t i = 4; i < 7; i++){ //check goals
        istringstream iss;
        int x;
        iss.str(argv[i]);

        if (!(iss >> x) || (x < 0)){ //check for goal being negative or a non-int
            string letter;
            switch (i)
            {
            case 4:{
                letter = "A";
                break;
            }
            case 5:{
                letter = "B";
                break;
            }
            case 6:{
                letter = "C";
                break;
            }
            default:
                break;
            }

            cout << "Error: Invalid goal '" << argv[i] << "' for jug " << letter << ".";
            return false;
        }
    }

    for (int i = 4; i < 7; i++)  //check for goal > cap
    {
        istringstream iss;
        iss.str(argv[i]);
        int x;
        int y;
        iss >> x;
        iss.clear();
        iss.str(argv[i-3]);
        iss >> y;
        if(x>y){    //check for goal > capacity
            string letter;
            switch (i)
            {
            case 4:{
                letter = "A";
                break;
            }
            case 5:{
                letter = "B";
                break;
            }
            case 6:{
                letter = "C";
                break;
            }
            default:
                break;
            }
            cout << "Error: Goal cannot exceed capacity of jug " << letter << ".";
            return false;
        }
    }
    int total_goal = 0;
    istringstream iss;
    int c;

    iss.str(argv[4]);
    iss >> c;
    total_goal += c;

    iss.clear();
    iss.str(argv[5]);
    iss >> c;
    total_goal += c;

    iss.clear();
    iss.str(argv[6]);
    iss >> c;
    total_goal += c;
    
    iss.clear();
    iss.str(argv[3]);
    iss >> c;

    if (total_goal != c)
    {
        cout << "Error: Total gallons in goal state must be equal to the capacity of jug C.";
        return false;
    }
    
    return true;
}

string print_steps(State* current,vector<vector<State*>> matrix){
    stack <State*> stack;

    while (current->parent != nullptr)
    {
        stack.push(current);
        current = current->parent;
    }
    stack.push(current);

    cout << "Initial state. " << current->to_string() << endl;
    stack.pop();

    while (!stack.empty())
    {
        current = stack.top();
        cout << current->directions << " " << current->to_string() << endl;
        stack.pop();
    }

    for (size_t i = 0; i < matrix.size(); i++)
    {
        for (size_t j = 0; j < matrix[i].size(); j++)
        {
            delete matrix[i][j]; 
        }
    }
    
    return "";
}

string find_state(int cap_a, int cap_b, int cap_c, int goal_a, int goal_b, int goal_c){
    State* current = new State(0,0,cap_c,"");
    queue <State*> states;
    vector<vector<State*>> matrix(cap_a+1);

    for (int i = 0; i <= cap_a; i++)//populate matrix to correct size
    {
        matrix[i] = vector<State*>(cap_b+1);
    }

    for (int i = 0; i <= cap_a; i++)//fill matrix with nullptr
    {
        for (int j = 0; j <= cap_b; j++)
        {
            matrix[i][j] = nullptr;
        }
    }

    states.push(current);
    while (!states.empty())
    {
        
        State* current = states.front();
        states.pop();
        if ((current->a==goal_a) && (current->b==goal_b) && (current->c==goal_c))
        {
            while (!states.empty())
            {
                delete states.front();
                states.pop();
            }
            matrix[current->a][current->b] = current;
            return print_steps(current,matrix);
        }
        if (matrix[current->a][current->b] != nullptr)
        {
            delete current;
            continue;
        }
        
        matrix[current->a][current->b] = current; // mark state as visited
    

        if (!(current->c == 0) && !(current->a == cap_a)) // pour from c to a DONE
        {
            //return to_string(current->a) + to_string(current->c);
            int c;
            int a;
            if (current->a+current->c > cap_a)
            {
                c = current->c - (cap_a - current->a);
                a = cap_a;
            }
            else
            {
                c = 0;
                a = current->c + current->a;
            }

            State* CtoA = new State(a, current->b, c, "");

            if((current->c-c) > 1){
                CtoA->directions = "Pour " + to_string(current->c-c) + " gallons from C to A.";
            }
            else
            {
                CtoA->directions = "Pour " + to_string(current->c-c) + " gallon from C to A.";
            }

            CtoA->parent = current;
            states.push(CtoA);
        }

        if (!(current->b == 0) && !(current->a == cap_a)) // pour from b to a 
        {
            int b;
            int a;
            if (current->a+current->b > cap_a)
            {
                b = current->b - (cap_a - current->a);
                a = cap_a;
            }
            else
            {
                b = 0;
                a = current->b + current->a;
            }

            State* BtoA = new State(a, b, current->c, "");

            if(current->b-b > 1){
                BtoA->directions = "Pour " + to_string(current->b-b) + " gallons from B to A.";
            }
            else
            {
                BtoA->directions = "Pour " + to_string(current->b-b) + " gallon from B to A.";
            }
            
            BtoA->parent = current;
            states.push(BtoA);
        }

        if (!(current->c == 0) && !(current->b == cap_b)) // pour from c to b done
        {

            int c;
            int b;

            if (current->b+current->c > cap_b)
            {
                c = current->c - (cap_b - current->b);
                b = cap_b;
            }
            else
            {
                c = 0;
                b = current->c + current->b;
            }

            State* CtoB = new State(current->a, b, c, "");

            if (current->c-c > 1)
            {
                CtoB->directions = "Pour " + to_string(current->c-c) + " gallons from C to B.";
            }
            else
            {
                CtoB->directions = "Pour " + to_string(current->c-c) + " gallon from C to B.";
            }
            
            CtoB->parent = current;
            states.push(CtoB);
        }
        if (!(current->a == 0) && !(current->b == cap_b)) // pour from a to b done
        {
            int a;
            int b;

            if (current->b+current->a > cap_b)
            {
                a = current->a - (cap_b - current->b);
                b = cap_b;
            }
            else
            {
                a = 0;
                b = current->a + current->b;
            }

            State* AtoB = new State(a, b, current->c, "");

            if (current->a-a > 1)
            {
                AtoB->directions = "Pour " + to_string(current->a-a) + " gallons from A to B.";
            }
            else
            {
                AtoB->directions = "Pour " + to_string(current->a-a) + " gallon from A to B.";
            }
            
            AtoB->parent = current;
            states.push(AtoB);

        }
        if (!(current->b == 0) && !(current->c == cap_c)) // pour from b to c
        {

            int b;
            int c;

            if (current->c+current->b > cap_c)
            {
                b = current->b - (cap_c - current->c);
                c = cap_c;
            }
            else
            {
                b = 0;
                c = current->b + current->c;
            }

            State* BtoC = new State(current->a, b, c, "");

            if (current->b-b > 1)
            {
                BtoC->directions = "Pour " + to_string(current->b-b) + " gallons from B to C.";
            }
            else
            {
                BtoC->directions = "Pour " + to_string(current->b-b) + " gallon from B to C.";
            }
            
            
            BtoC->parent = current;
            states.push(BtoC);

        }
        if (!(current->a == 0) && !(current->c == cap_c)) // pour from a to c
        {

            int a;
            int c;

            if (current->c+current->a > cap_c)
            {
                a = current->a - (cap_c - current->c);
                c = cap_c;
            }
            else
            {
                a = 0;
                c = current->a + current->c;
            }

            State* AtoC = new State(a, current->b, c, "");

            if (current->a-a > 1)
            {
                AtoC->directions = "Pour " + to_string(current->a-a) + " gallons from A to C.";
            }
            else
            {
                AtoC->directions = "Pour " + to_string(current->a-a) + " gallon from A to C.";
            }
            
            
            AtoC->parent = current;
            states.push(AtoC);
        }
    }

    while (!states.empty())
    {
        delete states.front();
        states.pop();
    }
    for (size_t i = 0; i < matrix.size(); i++)
    {
        for (size_t j = 0; j < matrix[i].size(); j++)
        {
            delete matrix[i][j]; 
        }
    }
    return "No solution.";
    
}

int main(int argc, char * const argv[]) {
    if(!check_input(argc,argv)){
        return 1;
    }

    istringstream iss;
    int cap_a;
    int cap_b;
    int cap_c;
    int goal_a;
    int goal_b;
    int goal_c;

    iss.str(argv[1]);
    iss >> cap_a;
    iss.clear();
    iss.str(argv[2]);
    iss >> cap_b;
    iss.clear();
    iss.str(argv[3]);
    iss >> cap_c;
    iss.clear();
    iss.str(argv[4]);
    iss >> goal_a;
    iss.clear();
    iss.str(argv[5]);
    iss >> goal_b;
    iss.clear();
    iss.str(argv[6]);
    iss >> goal_c;
    iss.clear();

    cout << find_state(cap_a,cap_b,cap_c,goal_a,goal_b,goal_c);



    return 0;
}