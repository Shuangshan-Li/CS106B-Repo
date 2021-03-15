#include "Plotter.h"
#include "strlib.h"
using namespace std;

void runPlotterScript(istream& input) {
    PenStyle style = {1, "black"};
    bool shouldDraw = false;
    double currentX = 0;
    double currentY = 0;

    for (string line; getline(input, line);) {
        Vector<string> instruction = stringSplit(line, " ");
        string command = toLowerCase(instruction[0]);
        switch (instruction.size()) {
            case 1:
                if (command == "pendown") {
                    shouldDraw = true;
                } else {
                    shouldDraw = false;
                }
                break;
            case 2:
                if (command == "pencolor") {
                    style.color = toLowerCase(instruction[1]);
                } else {
                    style.width = stringToReal(instruction[1]);
                }
            break;
            default:
                double nextX;
                double nextY;
                if (command == "moveabs") {
                    nextX = stringToReal(instruction[1]);
                    nextY = stringToReal(instruction[2]);
                } else {
                    nextX = stringToReal(instruction[1]) + currentX;
                    nextY = stringToReal(instruction[2]) + currentY;
                }
                if (shouldDraw) {
                    drawLine(currentX, currentY, nextX, nextY, style);
                }
                currentX = nextX;
                currentY = nextY;
        }
    }
}

