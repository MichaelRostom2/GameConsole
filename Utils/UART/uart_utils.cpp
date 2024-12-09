#include "uart_utils.h"

// Function to send new scores to computer
void sendNewScore(String game, int score)
{
    Serial.print("CMD:UPDATE ");
    Serial.print(game);
    Serial.print(" ");
    Serial.println(score);
}

// Function to process responses from the computer
void processResponse(String response, int &scoreToUpdate)
{
    if (response.startsWith("CMD:SCORE"))
    {
        int spaceIndex1 = response.indexOf(" ");
        int spaceIndex2 = response.indexOf(" ", spaceIndex1 + 1);
        String game = response.substring(spaceIndex1 + 1, spaceIndex2);
        int score = response.substring(spaceIndex2 + 1).toInt();

        scoreToUpdate = max(scoreToUpdate, score);

        // Serial.print("Updated Score for ");
        // Serial.print(game);
        // Serial.print(": ");
        Serial.println(score);
    }
    else
    {
        Serial.println("Unknown Response");
    }
}