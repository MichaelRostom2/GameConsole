#include "uart_utils.h"

/*!
    @brief  Sends a new score to the computer
    @param  game The game to update the score for
    @param  score The new score to send
*/
void sendNewScore(String game, int score)
{
    Serial.print("CMD:UPDATE ");
    Serial.print(game);
    Serial.print(" ");
    Serial.println(score);
}
/*!
    @brief  Processes a response from the computer
    @param  response The response to process
    @param  scoreToUpdate Pointer to the score to update
*/
void processResponse(String response, int &scoreToUpdate)
{
    if (response.startsWith("CMD:SCORE"))
    {
        int spaceIndex1 = response.indexOf(" ");
        int spaceIndex2 = response.indexOf(" ", spaceIndex1 + 1);
        String game = response.substring(spaceIndex1 + 1, spaceIndex2);
        int score = response.substring(spaceIndex2 + 1).toInt();

        scoreToUpdate = max(scoreToUpdate, score);
        Serial.println(score);
    }
    else
    {
        Serial.println("Unknown Response");
    }
}