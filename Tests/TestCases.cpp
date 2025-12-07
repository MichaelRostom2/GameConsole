#include "TestCases.h"

Ball testingBall = {0, 0, 0, 0, 0};
int testingPaddle_y_pos = 0;

/* Runs through all the test cases defined above */

bool testAllTests()
{
    delay(1000);
    Serial.println("Running all tests; ignore intermediate output...");
    if (!horizontalBounceTest() || !collideBallTest() || !updateBallTest() || !updatePaddleTest() || !pingUpdateFSMTest() || !updatePlayerTest() || !checkCollisionTest() || !spawnBulletTest() || !updateBulletsTest() || !switchToPingTest() || !switchToDodgeTest() || !updateFSMTest() || !DodgeUpdateFSMTest() || !resetISRTest() || !homeISRTest() || !processReponseTest())
    {
        return false;
    }

    Serial.println("All tests passed!");
    return true;
}

bool floatsEqual(float val1, float val2)
{
    const float EPSILON = .001;
    return (val1 - val2 > EPSILON || val2 - val1 > EPSILON) ? false : true;
}
void testFailed(String testName, float expected, float actual)
{
    Serial.print("Test failed! ");
    Serial.println(testName);
    Serial.print("Expected: ");
    Serial.println(expected);
    Serial.print("Actual: ");
    Serial.println(actual);
}
bool test(String testName, float expected, float actual)
{
    if (floatsEqual(expected, actual))
    {
        return true;
    }
    else
    {
        testFailed(testName, expected, actual);
        return false;
    }
}

/* Ping Unit Tests */
bool horizontalBounceTest()
{
    testingBall.dx = 1;
    testingBall.bounciness = 0.1f;
    horizontalBounce(testingBall);
    return test("horizontalBounceTest", -0.1f, testingBall.dx);
}

bool collideBallTest()
{
    // collidng with top wall
    testingBall.x = 120;
    testingBall.y = 0;
    testingBall.dy = -40;
    testingBall.dx = 1;
    CollideBall(testingBall, testingPaddle_y_pos);
    bool test1 = test("collideBallTest top wall", 40, testingBall.dy) && test("collideBallTest top wall", 0, testingBall.y);

    // colliding with bottom wall
    testingBall.x = 120;
    testingBall.y = 500;
    testingBall.dy = -40;
    testingBall.dx = 1;
    CollideBall(testingBall, testingPaddle_y_pos);
    bool test2 = test("collideBallTest bottom wall", 40, testingBall.dy) && test("collideBallTest bottom wall", 320 - 16, testingBall.y);

    // colliding with back wall
    testingBall.x = 240 - 8 - 16 - 1;
    testingBall.y = 0;
    testingBall.dy = -40;
    testingBall.dx = 1;
    CollideBall(testingBall, testingPaddle_y_pos);
    bool test3 = test("collideBallTest back wall", testingBall.x, 240 - 8 - 16 - 1);

    // colliding with paddle
    testingPaddle_y_pos = 0;
    testingBall.x = 0;
    testingBall.y = 0;
    testingBall.dy = -40;
    testingBall.dx = 1;
    float centerYDiff = (testingBall.y + 16 / 2) - (testingPaddle_y_pos + 64 / 2);
    CollideBall(testingBall, testingPaddle_y_pos);
    bool test4 = test("collideBallTest paddle", testingBall.dy, -40 + 4 * centerYDiff);

    // not coliding with anything
    testingPaddle_y_pos = 0;
    testingBall.x = 100;
    testingBall.y = 100;
    testingBall.dy = -40;
    testingBall.dx = 1;
    CollideBall(testingBall, testingPaddle_y_pos);
    bool test5 = test("collideBallTest no collision", testingBall.x, 100) && test("collideBallTest no collision", testingBall.y, 100) && test("collideBallTest no collision", testingBall.dy, -40) && test("collideBallTest no collision", testingBall.dx, 1);

    return test1 &&
           test2 &&
           test3 &&
           test4 &&
           test5;
}

bool updatePaddleTest()
{
    bool allTestsPassed = true;
    // Case 1: no movement
    Joystick_input Joystick_input = {0, 0};
    allTestsPassed = allTestsPassed && test("updatePaddle no movement", 100, updatePaddle(Joystick_input, 100));
    // Case 2: x movement (should be ignored)
    Joystick_input = {500, 0};
    allTestsPassed = allTestsPassed && test("updatePaddle x movement", 100, updatePaddle(Joystick_input, 100));
    // Case 3: upward y movement
    Joystick_input = {0, -500};
    allTestsPassed = allTestsPassed && test("updatePaddle upward movement", 97, updatePaddle(Joystick_input, 100));
    // Case 4: downward y movement
    Joystick_input = {0, 500};
    allTestsPassed = allTestsPassed && test("updatePaddle downward movement", 103, updatePaddle(Joystick_input, 100));
    // Case 5: upward y movement off-screen (should stay still)
    Joystick_input = {0, -500};
    allTestsPassed = allTestsPassed && test("updatePaddle upward movement off-screen", 0, updatePaddle(Joystick_input, 0));
    // Case 6: downward y movement off-screen (should stay still)
    Joystick_input = {0, 500};
    allTestsPassed = allTestsPassed && test("updatePaddle no movement", 320 - 64, updatePaddle(Joystick_input, 320 - 64));

    return allTestsPassed;
}

bool updateBallTest()
{
    float gravity = 0; // Initial gravity
    Ball testBall;
    bool allTestsPassed = true;

    // Ball with no initial velocity
    testBall = {50, 50, 0, 0, 0.1}; // dx and dy = 0
    gravity = 0;                    // No gravity initially

    updateBall(testBall, gravity);
    gravity += 90 * 0.1;               // Gravity increases in updateBall
    float expectedDx = gravity * 0.01; // dx = Gravity * 0.01
    float expectedX = 50 + expectedDx * 0.01;

    allTestsPassed = allTestsPassed && test("updateBall no velocity x", expectedX, testBall.x);
    allTestsPassed = allTestsPassed && test("updateBall no velocity y", 50, testBall.y);

    // Ball with initial velocity
    testBall = {50, 50, 0, -10, 0.1};
    gravity = 50;
    updateBall(testBall, gravity);
    gravity += 90 * 0.1; // Gravity updated again
    expectedDx = gravity * 0.01;
    expectedX = 50 + expectedDx * 0.01;
    float expectedY = 50 + (-10 * 0.01);

    allTestsPassed = allTestsPassed && test("updateBall with velocity x", expectedX, testBall.x);
    allTestsPassed = allTestsPassed && test("updateBall with velocity y", expectedY, testBall.y);

    // Ball after multiple updates
    testBall = {50, 50, 0, 0, 0.1};
    gravity = 0;
    // float expectedGravity
    float expectedXMulti = 50;
    float expectedYMulti = 50;
    float dx = 0.0f;
    for (int i = 0; i < 5; i++)
    {
        updateBall(testBall, gravity);
        dx += gravity * 0.01;
        expectedXMulti += dx * 0.01;
    }

    allTestsPassed = allTestsPassed && test("updateBall multi-update x", expectedXMulti, testBall.x);
    allTestsPassed = allTestsPassed && test("updateBall multi-update y", expectedYMulti, testBall.y);

    return allTestsPassed;
}

bool pingUpdateFSMTest()
{
    bool allTestsPassed = true;

    Joystick_input Joystick_input = {0, 0};
    allTestsPassed = allTestsPassed && test("pingUpdateFSM start state", PING_MOVE, PingUpdateFSM(PING_START, Joystick_input));

    allTestsPassed = allTestsPassed && test("pingUpdateFSM move state", PING_MOVE, PingUpdateFSM(PING_MOVE, Joystick_input));

    allTestsPassed = allTestsPassed && test("pingUpdateFSM game over state", PING_GAME_OVER, PingUpdateFSM(PING_GAME_OVER, Joystick_input));
    return allTestsPassed;
}

bool updatePlayerTest()
{
    bool allTestsPassed = true;
    Player testPlayer = {100, 100, true};

    Joystick_input Joystick_input = {0, 0};
    updatePlayer(Joystick_input, testPlayer);
    allTestsPassed = allTestsPassed && test("updatePlayer no movement", 100, testPlayer.x);

    testPlayer = {100, 100, true};
    Joystick_input = {500, 0};
    updatePlayer(Joystick_input, testPlayer);
    allTestsPassed = allTestsPassed && test("updatePlayer right movement", 100 + 1.5, testPlayer.x);

    testPlayer = {100, 100, true};
    Joystick_input = {-500, 0};
    updatePlayer(Joystick_input, testPlayer);
    allTestsPassed = allTestsPassed && test("updatePlayer left movement", 100 - 1.5, testPlayer.x);

    testPlayer = {100, 100, true};
    Joystick_input = {0, -500};
    updatePlayer(Joystick_input, testPlayer);
    allTestsPassed = allTestsPassed && test("updatePlayer left movement", 100 - 1.5, testPlayer.y);

    testPlayer = {100, 100, true};
    Joystick_input = {0, 500};
    updatePlayer(Joystick_input, testPlayer);
    allTestsPassed = allTestsPassed && test("updatePlayer left movement", 100 + 1.5, testPlayer.y);

    return allTestsPassed;
}

/* Dodge unit tests */
Player testPlayer = {0, 0, true};
Bullet testBullet = {0, 0, 0, 0, true};

bool checkCollisionTest()
{

    // bullet is inactive
    testBullet.active = false;

    bool test1 = test("checkCollisionTest inactive bullet", false, checkCollision(testBullet, testPlayer));

    // no collision
    testBullet.x = 100;
    testBullet.y = 100;
    testBullet.active = true;
    testPlayer.x = 0;
    testPlayer.y = 0;
    testPlayer.alive = true;

    bool test2 = test("checkCollisionTest no collision", false, checkCollision(testBullet, testPlayer));

    // bullet colliding left
    testBullet.x = 100;
    testBullet.y = 100;
    testBullet.active = true;
    testPlayer.x = 100 + 11;
    testPlayer.y = 100;
    testPlayer.alive = true;

    bool test3 = test("checkCollisionTest collide left", true, checkCollision(testBullet, testPlayer));

    // bullet colliding right
    testBullet.x = 100 + 15;
    testBullet.y = 100;
    testBullet.active = true;
    testPlayer.x = 100;
    testPlayer.y = 100;
    testPlayer.alive = true;

    bool test4 = test("checkCollisionTest collide right", true, checkCollision(testBullet, testPlayer));

    // bullet colliding top
    testBullet.x = 100;
    testBullet.y = 100;
    testBullet.active = true;
    testPlayer.x = 100;
    testPlayer.y = 100 - 11;
    testPlayer.alive = true;

    bool test5 = test("checkCollisionTest collide top", true, checkCollision(testBullet, testPlayer));

    // bullet colliding bottom
    testBullet.x = 100;
    testBullet.y = 100 + 15;
    testBullet.active = true;
    testPlayer.x = 100;
    testPlayer.y = 100;
    testPlayer.alive = true;

    bool test6 = test("checkCollisionTest collide bottom", true, checkCollision(testBullet, testPlayer));

    return test1 && test2 && test3 && test4 && test5 && test6;
}

bool spawnBulletTest()
{
    bool allTestsPassed = true;
    int numBulletsSpawned = 0;
    spawnBullet();
    for (int i = 0; i < maxBullets; i++)
    {
        if (bullets[i].active)
        {
            numBulletsSpawned++;
        }
    }
    allTestsPassed = allTestsPassed && test("spawnBullet spawn one bullet", 1, numBulletsSpawned);
    spawnBullet();
    spawnBullet();
    spawnBullet();
    spawnBullet();
    spawnBullet();
    numBulletsSpawned = 0;
    for (int i = 0; i < maxBullets; i++)
    {
        if (bullets[i].active)
        {
            numBulletsSpawned++;
        }
    }
    allTestsPassed = allTestsPassed && test("spawnBullet spawn 6 bullet", 6, numBulletsSpawned);

    return allTestsPassed;
}
bool checkBulletCollisionsTest()
{
    testBullet = {-100, -100, 0, 0, true};
    testPlayer = {0, 0, true};
    bullets[0] = testBullet;
    bullets[1] = {500, 500, 0, 0, true};

    checkBulletCollisions();

    return test("checkCollisionTest inactive bullet", false, bullets[0].active) &&
           test("checkCollisionTest inactive bullet", false, bullets[1].active);
}

bool switchToPingTest()
{
    bool allTestsPassed = true;
    currentGame = PING;
    PING_CURRENT_STATE = PING_START;
    switchToPing();
    allTestsPassed = allTestsPassed && test("switchToPing currentGame changed", PING, currentGame);
    allTestsPassed = allTestsPassed && test("switchToPing PING_CURRENT_STATE changed", PING_START, PING_CURRENT_STATE);

    return allTestsPassed;
}

bool switchToDodgeTest()
{
    bool allTestsPassed = true;
    currentGame = DODGE;
    DODGE_CURRENT_STATE = DODGE_START;
    switchToDodge();
    allTestsPassed = allTestsPassed && test("switchToDodge currentGame changed", DODGE, currentGame);
    allTestsPassed = allTestsPassed && test("switchToDodge DODGE_CURRENT_STATE changed", DODGE_START, DODGE_CURRENT_STATE);

    return allTestsPassed;
}

bool resetISRTest()
{
    bool allTestsPassed = true;
    currentGame = DODGE;
    DODGE_CURRENT_STATE = DODGE_MOVE;
    resetISR();
    allTestsPassed = allTestsPassed && test("resetISR currentGame unchanged on DODGE", DODGE, currentGame);
    allTestsPassed = allTestsPassed && test("resetISR DODGE_CURRENT_STATE changed to START", DODGE_START, DODGE_CURRENT_STATE);

    currentGame = PING;
    PING_CURRENT_STATE = PING_MOVE;
    resetISR();
    allTestsPassed = allTestsPassed && test("resetISR currentGame unchanged on PING", PING, currentGame);
    allTestsPassed = allTestsPassed && test("resetISR PING_CURRENT_STATE changed to START", PING_START, PING_CURRENT_STATE);

    currentGame = MENU;
    resetISR();
    allTestsPassed = allTestsPassed && test("resetISR currentGame unchanged on MENU", MENU, currentGame);

    return allTestsPassed;
}

bool homeISRTest()
{
    bool allTestsPassed = true;
    currentGame = DODGE;
    homeISR();
    allTestsPassed = allTestsPassed && test("homeISR currentGame set to MENU on DODGE", MENU, currentGame);

    currentGame = PING;
    homeISR();
    allTestsPassed = allTestsPassed && test("homeISR currentGame set to MENU on PING", MENU, currentGame);

    currentGame = MENU;
    homeISR();
    allTestsPassed = allTestsPassed && test("resetISR currentGame unchanged on MENU", MENU, currentGame);

    return allTestsPassed;
}

bool updateBulletsTest()
{
    testBullet = {0, 0, 100, 100, true};
    testPlayer = {0, 0, true};
    bullets[0] = testBullet;

    updateBullets();

    return test("updateBulletsTest", 1, bullets[0].x) &&
           test("updateBulletsTest", 1, bullets[0].y) &&
           test("updateBulletsTest", 100, bullets[0].dx) &&
           test("updateBulletsTest", 100, bullets[0].dy);
}
bool updateFSMTest()
{
    Joystick_input joystickInput = {0, 0};
    currentGame = MENU;
    updateFSM(joystickInput, 0.01);
    bool test1 = test("updateFSMTest", MENU, currentGame);

    joystickInput = {400, 0};
    currentGame = MENU;
    updateFSM(joystickInput, 0.01);
    bool test2 = test("updateFSMTest", PING, currentGame);

    joystickInput = {-400, 0};
    currentGame = MENU;
    updateFSM(joystickInput, 0.01);
    bool test3 = test("updateFSMTest", DODGE, currentGame);

    joystickInput = {0, 0};
    currentGame = PING;
    updateFSM(joystickInput, 0.01);
    bool test4 = test("updateFSMTest", PING, currentGame);

    joystickInput = {0, 0};
    currentGame = DODGE;
    updateFSM(joystickInput, 0.01);
    bool test5 = test("updateFSMTest", DODGE, currentGame);

    return test1 && test2 && test3 && test4 && test5;
}
bool DodgeUpdateFSMTest()
{
    bool allTestsPassed = true;

    Joystick_input Joystick_input = {0, 0};
    allTestsPassed = allTestsPassed && test("DodgeUpdateFSM start state", DODGE_MOVE, DodgeUpdateFSM(DODGE_START, Joystick_input));

    allTestsPassed = allTestsPassed && test("DodgeUpdateFSM move state", DODGE_MOVE, DodgeUpdateFSM(DODGE_MOVE, Joystick_input));

    allTestsPassed = allTestsPassed && test("DodgeUpdateFSM game over state", DODGE_GAME_OVER, DodgeUpdateFSM(DODGE_GAME_OVER, Joystick_input));
    return allTestsPassed;
}
bool processReponseTest()
{
    char response1[20] = "CMD:SCORE PING 100";
    int score = 0;
    processResponse(response1, score);
    bool test1 = test("processReponseTest", 100, score);

    char response2[20] = "CMD:SCORE DODGE 200";
    processResponse(response2, score);
    bool test2 = test("processReponseTest", 200, score);
    return test1 && test2;
}