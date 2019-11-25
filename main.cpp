// Data Analysis and Visualization: File I/O, Streams and Arrays
// Description of program:
// This program displays a state's total energy consumption for coal, natural gas,
// petroleum, nuclear and renewables from a file called Data2015.csv.
//  and creates an image in ppm format which will represent the percentage of energy consumption per state.
// It will query the preprocessed data and display the state's energy consumption
// as a percentage of that state's total energy consumption.


#include <iostream>
#include <fstream> //Library for ifstream and ofstream
#include <sstream>
#include <string>
#include <iomanip> //Library for setprecision()

using namespace std;

const int STATES=51;  //Declaration of constant:STATES
const int COLUMN=15;  //Declaration of constant:COLUMN
const int HEIGHT=480; //Declaration of constant:HEIGHT
const int WIDTH=640;  //Declaration of constant:WIDTH
const int RGB=3;      //Declaration of constant:RGB

enum Colors {RED, GREEN, BLUE};  //An enum called Colors to track RGB

struct Data {    // This is a struct type called Data with a string variable called statename and a double array called energy
    string statename;   //This is used to store the states
    double energy[COLUMN];  // Energy array with 15 elements of type double, as we have 14 energy columns and one state column
};


void ReadData(ifstream &mydata, Data myArray[], int i);  //Declaration of function: ReadData
void WriteImage(std::ofstream &out, unsigned char pixels[][WIDTH][RGB],double p1, double p2, double p3, double p4, double p5);  //Declaration of function: WriteImage


int main() {
    
    ifstream mydata;  //This is used to contain the data from the file
    ofstream myimage; //This is used to contain the data to store in the file
    
    Data answer;   //This is used to contain the user input
    Data myArray[STATES];   //Array is used to store the states
    
    int y,x;
    double coal=0, natural=0,petrol=0,nuclear=0,renewable=0;  //Declaration for the five energy groups
    double myTotal=0;  //This is used to store the state total energy consumption for calculating the percentage
    double coal_frac,natural_frac,petrol_frac,nuclear_frac,renewable_frac;      //These variables are used to store the fraction of the energry
    unsigned char pixels[HEIGHT][WIDTH][RGB];       //This is a 3D array to store the every single pixel
    
    cout<<"Enter a statename or'q' to exits:"<<endl;
    getline(cin,answer.statename,'\n');   // Using getline(cin,) instead of cin only as cin stop when it meets whitespace
    
    
    while (answer.statename!="q") {  //Code excutes when answer is not equal to q
        ReadData(mydata, myArray, STATES);
        
        for (y=0; y<STATES; y++) {   //for loop to find the same state from the arrays that the user asked for
            
            if (answer.statename==myArray[y].statename) {   //Executes if the state is found
                
                coal=myArray[y].energy[1];  //energy[1] contains the data of the 1st energy: Coal
                natural=myArray[y].energy[2];  //energy[2] contains the data of the 2nd energy: Natural Gas
                
                //energy array 3 to 7 are: Fuel Oil, Jet Fuel, LPG, Motor Gasoline, Residual Fuel Oil
                for (x=3; x<8; x++) {
                    petrol= petrol + myArray[y].energy[x];
                }
                
                nuclear=myArray[y].energy[8];  //energy[8] contains the data of the 8th energy: Nuclear Electric Power
                
                //energy array 9 to 14 are: Hydroelectric Power, Wood and Waste, Fuel Ethanol, Geothermal, Solar, Wind
                for (x=9; x<COLUMN; x++) {
                    renewable= renewable + myArray[y].energy[x];
                }
                
                myTotal=myArray[y].energy[0]; // energy[0] contains the data of the total energy consumption
                
                coal_frac= coal/myTotal;
                natural_frac=natural/myTotal;
                petrol_frac=petrol/myTotal;
                nuclear_frac=nuclear/myTotal;
                renewable_frac=renewable/myTotal;
                
                //Print out the data by two decimals
                cout<<fixed<<setprecision(2)<<"Coal: "<<coal_frac*100<<"%"<<'\n'
                <<"Natural Gas: "<<natural_frac*100<<"%"<<'\n'
                <<"Petroleum: "<<petrol_frac*100<<"%"<<'\n'
                <<"Nuclear: "<<nuclear_frac*100<<"%"<<'\n'
                <<"Renewables: "<<renewable_frac*100<<"%"<<'\n';
                
                WriteImage(myimage, pixels,coal_frac, natural_frac, petrol_frac, nuclear_frac, renewable_frac);  //Calling function WriteImage
                
                petrol=0;renewable=0; //These two variables are assigned to 0 for another round of searching
            }
        }
        
        cout<<"Enter a statename or 'q' to exits:"<<endl;
        getline(cin,answer.statename,'\n');
    }
    
    if (answer.statename=="q") {  //if answer is equal to q, cout Goodbye
        cout<<"Goodbye";
    }
    
    return 0;
}


void ReadData(ifstream &mydata, Data myArray[], int STATES)  //This function reads and stores the data into arrays
{
    int y,x;
    int lastColumn=14;
    double myTotal=0;
    string header;
    string xline;  //This is used to store the temporary data of numbers before it assigned to the arrays
    
    mydata.open("2015Data.csv");  //Open the csv file
    
    if (!mydata){  //If file is not open
        cout<<"File read error.";
        
    }
    
    getline(mydata,header,'\n'); // To skip the header
    
    for (y=0; y<STATES; y++) {  //This for loop is used to store the statename in the array
        
        getline(mydata,myArray[y].statename,',');   //Read until it reached a comma and store it in array.statename
        
        for (x=1; x<COLUMN; x++) {  //This for loop is used to store the energy in the array
            
            if(x==lastColumn){      //When it reached the last column, reads until new line and store it in xline(a temporary space)
                getline(mydata,xline,'\n');
            }
            else getline(mydata,xline,',');     //Else reads until comma and store it in xline
            double myDoub=stod(xline);   //Convert xline from string to double
            
            myArray[y].energy[x] =myDoub;   //Assign to the energy arrays
            myTotal=myTotal + myArray[y].energy[x];     //Adding all the energy to calculate the percentage
            
        }
        
        myArray[y].energy[0]=myTotal;    //The total is stored in energy[0]
        myTotal=0;   //Assign myTotal to 0 to store another state total
    }
    
    mydata.close();  //Closing file
}

//This function create bar chart and store the data into an image, called graph.ppm
void WriteImage(std::ofstream &out, unsigned char pixels[][WIDTH][RGB],double p1, double p2, double p3, double p4, double p5)
{
    out.open("graph.ppm");  //open or create graph.ppm if it does not exist
    
    if (out.fail()) { //If the file is not open
        cout<<"output file opening failed.\n";
    }
    
    out<<"P3\n640 480\n255\n";   //Store ppm format with P3, Width and Height
    int y,x;
    for (y=0; y<HEIGHT; y++) {   //Writes from the 1st row to 480th row
        for (x=0; x<WIDTH; x++) {   //Writes from 1st column to 640th column
            
           /* if (x<=p1*WIDTH) {
                pixels[y][x][RED]=0;
                pixels[y][x][GREEN]=0;
                pixels[y][x][BLUE]=0;
            }
            else
            {
                pixels[y][x][RED]=127;
                pixels[y][x][GREEN]=127;
                pixels[y][x][BLUE]=127;
            }
            */
            if (x<=WIDTH/5) {   //1st bar of the graph: coal
                if (y<=(1-p1)*HEIGHT) {  //Uses 1-p1 instead of p1 else will get a flip graph
                    pixels[y][x][RED]=127;
                    pixels[y][x][GREEN]=127;
                    pixels[y][x][BLUE]=127;
                }else{   //Writes red pixels
                    pixels[y][x][RED]=255;
                    pixels[y][x][GREEN]=0;
                    pixels[y][x][BLUE]=0;
                }
            }
            else if(x>WIDTH/5 && x<=WIDTH/5*2){  //2nd bar of the graph: Natural Gas
                if (y<=(1-p2)*HEIGHT) {
                    pixels[y][x][RED]=127;
                    pixels[y][x][GREEN]=127;
                    pixels[y][x][BLUE]=127;
                }else{   //Writes green pixels
                    pixels[y][x][RED]=0;
                    pixels[y][x][GREEN]=255;
                    pixels[y][x][BLUE]=0;
                }
            }
            else if (x>WIDTH/5*2 && x<=WIDTH/5*3){   //3rd bar of the graph: Petroleum
                if (y<=(1-p3)*HEIGHT) {
                    pixels[y][x][RED]=127;
                    pixels[y][x][GREEN]=127;
                    pixels[y][x][BLUE]=127;
                }else{    //Writes blue pixels
                    pixels[y][x][RED]=0;
                    pixels[y][x][GREEN]=0;
                    pixels[y][x][BLUE]=255;
                }
            }
            else if (x>WIDTH/5*3 && x<=WIDTH/5*4){      //4th bar of the graph: Nuclear
                if (y<=(1-p4)*HEIGHT) {
                    pixels[y][x][RED]=127;
                    pixels[y][x][GREEN]=127;
                    pixels[y][x][BLUE]=127;
                }else{     //Writes pink pixels
                    pixels[y][x][RED]=255;
                    pixels[y][x][GREEN]=20;
                    pixels[y][x][BLUE]=147;
                }
            }
            else if (x>WIDTH/5*4 && x<=WIDTH){      //5th bar of the graph: Renewable
                if (y<=(1-p5)*HEIGHT) {
                    pixels[y][x][RED]=127;
                    pixels[y][x][GREEN]=127;
                    pixels[y][x][BLUE]=127;
                }else{      //Writes yellow pixels
                    pixels[y][x][RED]=255;
                    pixels[y][x][GREEN]=255;
                    pixels[y][x][BLUE]=0;
                }
            }
            out<<(int)pixels[y][x][RED]<<" "<<(int)pixels[y][x][GREEN]<<" "<<(int)pixels[y][x][BLUE]<<" ";   //Writes pixels in integer to the file
        }
        out<<endl;  //Writes new line for every row
    }
    out.close();   //Closing file
}

