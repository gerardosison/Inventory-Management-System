#include <stdio.h>      // this includes the standard input/output library for using functions (ex. printf and scanf).
#include <string.h>     // Includes the string manipulation library, useful for string functions (ex. strcpy, strcat, etc).
#include <stdlib.h>     // Includes the standard library for memory allocation and other general-purpose functions.
#include "raylib.h"     // Includes the Raylib library for game development, providing functions to create windows, draw shapes, etc.
#include <ctype.h>      // Includes the character handling library, useful for functions like isdigit, isalpha, etc.
#include <time.h>       // Includes the time library, used for working with time and date, like getting current time.

#define MAX_PRODUCTS 100      // Defines a constant for the maximum number of products allowed, set to 100.
#define MAX_USERNAME_LENGTH 50 // Defines the maximum allowed length for usernames, set to 50 characters.
#define MAX_PASSWORD_LENGTH 50 // Defines the maximum allowed length for passwords, set to 50 characters.
#define MAX_USERS 10          // Defines the maximum number of users the system can handle, set to 10.
#define MAX_INVENTORY_SIZE 100 // Defines the maximum number of items allowed in the inventory, set to 100.
#define MAX_RECORDS 25        // Defines the maximum number of records that can be stored, set to 25.
#define MAX_ITEMS 30          // Defines the maximum number of items in each record, set to 30.
#define TEXT_COLOR BLACK      // Defines a color for text, setting it to black.
#define BUTTON_COLOR DARKGRAY // Defines a color for buttons, setting it to dark gray.
#define BUTTON_HOVER_COLOR LIGHTGRAY // Defines a color for buttons when hovered over, setting it to light gray.
#define KEY_1 KEY_ONE        // Defines a key constant for the "1" key.
#define KEY_2 KEY_TWO        // Defines a key constant for the "2" key.

// --- Type definitions (must come before the prototypes below, since several
// prototypes reference User / StockRecord) ---

// User structure
typedef struct
{
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} User;

// Item structure
typedef struct
{
    char id[100];
    char name[100];
    int quantity;
    float price;
    int numSold;
    float sales;
    char log[500];
    float initialQuantity;
} Item;

typedef struct
{
    char recordName[50];
    Item items[MAX_ITEMS];
    int itemCount;
} StockRecord;

// Function Prototypes
void loadRecordsFromFile(const char *filename);
void saveRecordsToFile(const char *filename);
int strcasestr_index(const char *haystack, const char *needle);
void trimSpaces(char* str);
int compareByName(const void *a, const void *b);
int compareByPrice(const void *a, const void *b);
int getIntegerInput(const char *prompt, int x, int y);
float getFloatInput(const char *prompt, int x, int y);
void DrawCenteredText(const char *text, int y, int fontSize, Color color);
void handleInput(char *buffer, int maxLength);
void handleTextInput(char *input, int maxLength, int x, int y, int width, int height, Color textColor, Color boxColor);
void handleNumericInput(char *inputBuffer, int bufferSize, int x, int y, int width, int height, Color textColor, Color bgColor);
bool DrawBackButton(int x, int y, int width, int height);
void DrawScaledBackground(Texture2D background, int screenWidth, int screenHeight);
void DrawInputCentered(const char *label, const char *input, int y, int fontSize, Color labelColor, Color inputColor);
int login(User users[], int userCount);
void CreateRecord();
void addProd();
void deleteProd(int index);
void renameProd(int index);
void editPrice(int index);
void editID(int index);
void purchaseProd(int index);
void restockProd(int index);
void ViewCurrentRecord();
void renameRecord(int recordIndex);
void SwitchRecord();
void DeleteRecord();
void saveSummaryToFile(StockRecord *currentRecord);
void summaryRecord();
void CustomWaitTime(double seconds);
void showMessage(const char *message);

int itemCount = 0;
int purchaseQuantity = 0;

StockRecord records[MAX_RECORDS];
int recordCount = 0;
int currentRecordIndex = -1;


int main()
{
    User users[2] = {{"raiven", "admin"}, {"gerardo", "admin"}};
    int userCount = 2;

    if (!login(users, userCount))
    {
        printf("Login failed or user quit.\n");
        return 0;
    }

    InitWindow(1920, 1020, "Inventory Management System\n");
    SetExitKey(0);
    SetTargetFPS(60);
    loadRecordsFromFile("C:\\Users\\Rea Mae\\OneDrive\\Desktop\\recording.txt");
    Texture2D background = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\main1.jpg");
    if (background.id == 0)
    {
        showMessage("Failed to load background image!");
        CloseWindow();
        return -1;
    }

    int exitProgram = 0;

    float startX = 100;
    float startY = 200;
    float optionWidth = 600;
    float optionHeight = 40;
    float optionSpacing = 10;

    Rectangle option1 = {startX, startY + (optionHeight + optionSpacing) * 0, optionWidth, optionHeight};
    Rectangle option2 = {startX, startY + (optionHeight + optionSpacing) * 1, optionWidth, optionHeight};
    Rectangle option3 = {startX, startY + (optionHeight + optionSpacing) * 2, optionWidth, optionHeight};
    Rectangle option4 = {startX, startY + (optionHeight + optionSpacing) * 3, optionWidth, optionHeight};
    Rectangle option5 = {startX, startY + (optionHeight + optionSpacing) * 4, optionWidth, optionHeight};
    Rectangle option6 = {startX, startY + (optionHeight + optionSpacing) * 5, optionWidth, optionHeight};

    while (!WindowShouldClose() && !exitProgram)
    {
        Vector2 mousePosition = GetMousePosition();

        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        DrawScaledBackground(background, GetScreenWidth(), GetScreenHeight());
        DrawCenteredText("INVENTORY MANAGEMENT SYSTEM", 23, 60, WHITE);
        DrawCenteredText("INVENTORY MANAGEMENT SYSTEM", 20, 60, BLACK);
        DrawText("1. Create New Record", option1.x + 20, option1.y + 10, 50, CheckCollisionPointRec(mousePosition, option1) ? WHITE : BLACK);
        DrawText("2. Display Record", option2.x + 20, option2.y + 10, 50, CheckCollisionPointRec(mousePosition, option2) ? WHITE : BLACK);
        DrawText("3. Switch Record", option3.x + 20, option3.y + 10, 50, CheckCollisionPointRec(mousePosition, option3) ? WHITE : BLACK);
        DrawText("4. Delete Record", option4.x + 20, option4.y + 10, 50, CheckCollisionPointRec(mousePosition, option4) ? WHITE : BLACK);
        DrawText("5. Generate Summary of Transaction", option5.x + 20, option5.y + 20, 50, CheckCollisionPointRec(mousePosition, option5) ? WHITE : BLACK);
        DrawText("6. Exit", option6.x + 20, option6.y + 20, 50, CheckCollisionPointRec(mousePosition, option6) ? WHITE : BLACK);

        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(mousePosition, option1))
                CreateRecord();
            else if (CheckCollisionPointRec(mousePosition, option2))
                ViewCurrentRecord();
            else if (CheckCollisionPointRec(mousePosition, option3))
                SwitchRecord();
            else if (CheckCollisionPointRec(mousePosition, option4))
                DeleteRecord();
            else if (CheckCollisionPointRec(mousePosition, option5))
                summaryRecord ();
            else if (CheckCollisionPointRec(mousePosition, option6))
                exitProgram = 1;
        }
    }

    saveRecordsToFile("C:\\Users\\Rea Mae\\OneDrive\\Desktop\\recording.txt");
    UnloadTexture(background);
    CloseWindow();

    return 0;
}

void loadRecordsFromFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fscanf(file, "%d", &recordCount);
        for (int i = 0; i < recordCount; i++) {
            fscanf(file, "%s", records[i].recordName);
            fscanf(file, "%d", &records[i].itemCount);

            for (int j = 0; j < records[i].itemCount; j++) {
                fscanf(file, "%s %s %d %f %d %f",
                       records[i].items[j].id,
                       records[i].items[j].name,
                       &records[i].items[j].quantity,
                       &records[i].items[j].price,
                       &records[i].items[j].numSold,
                       &records[i].items[j].sales);
                records[i].items[j].log[0] = '\0';
                records[i].items[j].initialQuantity = records[i].items[j].quantity;
            }
        }
        fclose(file);
    }
}
// Function to save records and product details to a file
void saveRecordsToFile(const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file != NULL)
    {
        fprintf(file, "%d\n", recordCount);
        for (int i = 0; i < recordCount; i++)
        {
            fprintf(file, "%s %d\n", records[i].recordName, records[i].itemCount);

            for (int j = 0; j < records[i].itemCount; j++) {
                fprintf(file, "%s %s %d %.2f %d %.2f\n",
                        records[i].items[j].id,
                        records[i].items[j].name,
                        records[i].items[j].quantity,
                        records[i].items[j].price,
                        records[i].items[j].numSold,
                        records[i].items[j].sales);
            }
        }
        fclose(file);
    }
}
// Portable case-insensitive compare (avoids relying on POSIX strcasecmp,
// which isn't guaranteed available on plain Windows/MinGW builds)
int caseInsensitiveCompare(const char *a, const char *b)
{
    while (*a && *b)
    {
        int ca = tolower((unsigned char)*a);
        int cb = tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

//case sensitive
int strcasestr_index(const char *haystack, const char *needle)
{
    for (int i = 0; haystack[i] != '\0'; i++)
    {
        int j = 0;
        while (tolower(haystack[i + j]) == tolower(needle[j]) && needle[j] != '\0')
        {
            j++;
        }
            if (needle[j] == '\0')
            {
                return i;
            }
    }
    return -1;
}

//remove all spaces from the given string
void trimSpaces(char* str)
{
    int i = 0, j = 0;
    while (str[i] != '\0')
    {
        if (str[i] != ' ')
        {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0';
}

//alphabetical sorting
int compareByName(const void *a, const void *b)
{
    Item *itemA = (Item *)a;
    Item *itemB = (Item *)b;

    return strcmp(itemA->name, itemB->name);
}

//price sorting (back to alphabetical if same price)
int compareByPrice(const void *a, const void *b)
{
    Item *itemA = (Item *)a;
    Item *itemB = (Item *)b;

    if (itemA->price < itemB->price)
        return -1;
    else if (itemA->price > itemB->price)
        return 1;
    else
        return strcmp(itemA->name, itemB->name);
}

//handle the integer input from keyboard
int getIntegerInput(const char *prompt, int x, int y)
{
    char inputText[20] = {0};
    int inputValue = 0;
    int offsetX = x, offsetY = y;

    while (!IsKeyPressed(KEY_ENTER))
    {
        ClearBackground(RAYWHITE);

        DrawText(prompt, offsetX, offsetY, 20, DARKGRAY);
        DrawText(inputText, offsetX, offsetY + 30, 20, BLACK);

        for (int i = 48; i <= 57; i++)
        {
            if (IsKeyPressed(i))
            {
                char digit = (char)i;
                int len = strlen(inputText);
                if (len < sizeof(inputText) - 1)
                 {
                    inputText[len] = digit;
                    inputText[len + 1] = '\0';
                }
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE) && strlen(inputText) > 0)
        {
            inputText[strlen(inputText) - 1] = '\0';
        }

        BeginDrawing();
        EndDrawing();
    }

    if (strlen(inputText) > 0)
    {
        inputValue = atoi(inputText);
    }

    return inputValue;
}

//accept decimal point
float getFloatInput(const char *prompt, int x, int y)
 {
    char inputText[20] = {0};
    float inputValue = 0.0f;
    int offsetX = x, offsetY = y;

    while (!IsKeyPressed(KEY_ENTER))
    {
        ClearBackground(RAYWHITE);

        DrawText(prompt, offsetX, offsetY, 20, DARKGRAY);
        DrawText(inputText, offsetX, offsetY + 30, 20, BLACK);

        for (int i = 48; i <= 57; i++)
        {
            if (IsKeyPressed(i))
            {
                char digit = (char)i;
                int len = strlen(inputText);
                if (len < sizeof(inputText) - 1)
                {
                    inputText[len] = digit;
                    inputText[len + 1] = '\0';
                }
            }
        }

        if (IsKeyPressed(KEY_PERIOD) && strchr(inputText, '.') == NULL)
        {
            int len = strlen(inputText);
            if (len < sizeof(inputText) - 1)
            {
                inputText[len] = '.';
                inputText[len + 1] = '\0';
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE) && strlen(inputText) > 0)
        {
            inputText[strlen(inputText) - 1] = '\0';
        }

        BeginDrawing();
        EndDrawing();
    }

    if (strlen(inputText) > 0)
    {
        inputValue = atof(inputText);
    }

    return inputValue;
}

//center the text
void DrawCenteredText(const char *text, int y, int fontSize, Color color)
{
    int screenWidth = GetScreenWidth();
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, (screenWidth - textWidth) / 2, y, fontSize, color);
}

//wait time
void CustomWaitTime(double seconds)
{
    double start = GetTime();
    while (GetTime() - start < seconds) {}
}

//display message for 2 sec
void showMessage(const char *message)
 {
    BeginDrawing();
    ClearBackground(LIGHTGRAY);
    DrawCenteredText(message, GetScreenHeight() / 2, 30, BLACK);
    EndDrawing();
    CustomWaitTime(2);
}

//It handles character input, appending each character to the buffer (key board input)
void handleInput(char *buffer, int maxLength)
{
    int key = GetCharPressed();

    while (key > 0)
    {
        if (key >= 32 && key <= 125 && strlen(buffer) < maxLength - 1)
        {
            char temp[2] = {(char)key, '\0'};
            strcat(buffer, temp);
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && strlen(buffer) > 0)
    {
        buffer[strlen(buffer) - 1] = '\0';
    }
}

//for handling numbers
void handleNumericInput(char *inputBuffer, int bufferSize, int x, int y, int width, int height, Color textColor, Color bgColor)

{
    int key = GetCharPressed();

    while (key > 0)
    {
        if ((key >= '0' && key <= '9') || key == '.')
        {
            if (key == '.' && strchr(inputBuffer, '.') != NULL)
            {
                key = 0;
            }
            else
            {
                int len = strlen(inputBuffer);
                if (len < bufferSize - 1)
                {
                    inputBuffer[len] = (char)key;
                    inputBuffer[len + 1] = '\0';
                }
            }
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && strlen(inputBuffer) > 0)
    {
        inputBuffer[strlen(inputBuffer) - 1] = '\0';
    }

    DrawRectangle(x, y, width, height, bgColor);
    DrawRectangleLines(x, y, width, height, BLACK);
    DrawText(inputBuffer, x + 5, y + (height - 20) / 2, 20, textColor);
}

//handle input in the text box
void handleTextInput(char *input, int maxLength, int x, int y, int width, int height, Color textColor, Color boxColor)
{
    DrawRectangle(x, y, width, height, boxColor);
    DrawRectangleLines(x, y, width, height, DARKGRAY);

    DrawText(input, x + 10, y + (height / 4), 20, textColor);

    int key = GetCharPressed();

    while (key > 0)
    {
        if (key >= 32 && key <= 125)
        {
            if (strlen(input) < maxLength - 1)
            {
                char temp[2] = {(char)key, '\0'};
                strcat(input, temp);
            }
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && strlen(input) > 0)
    {
        input[strlen(input) - 1] = '\0';
    }
}

//back button function
bool DrawBackButton(int x, int y, int width, int height)
{
    Rectangle backButton = { x, y, width, height };
    Color buttonColor = BLACK;

    if (CheckCollisionPointRec(GetMousePosition(), backButton))
    {
        buttonColor = GRAY;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            return true;
        }
    }

    DrawRectangleRec(backButton, buttonColor);
    DrawRectangleLinesEx(backButton, 2, BLACK);
    DrawText("BACK", x + 10, y + 5, 20, WHITE);

    return false;
}

// Function to scale and draw the background image
void DrawScaledBackground(Texture2D texture, int screenWidth, int screenHeight)

 {
    float scaleX = (float)screenWidth / texture.width;
    float scaleY = (float)screenHeight / texture.height;

    Rectangle destRect = { 0, 0, screenWidth, screenHeight };

    Rectangle sourceRect = { 0, 0, texture.width, texture.height };

    DrawTexturePro(texture, sourceRect, destRect, (Vector2){ 0, 0 }, 0.0f, WHITE);
}
void DrawInputCentered(const char *label, const char *input, int y, int fontSize, Color labelColor, Color inputColor)
{
    int screenWidth = GetScreenWidth();

    int labelWidth = MeasureText(label, fontSize);

    int inputWidth = MeasureText(input, fontSize);

    int totalWidth = labelWidth + 10 + inputWidth;
    int startX = (screenWidth - totalWidth) / 2;

    DrawText(label, startX, y, fontSize, labelColor);
    DrawText(input, startX + labelWidth + 10, y, fontSize, inputColor);
}
int login(User users[], int userCount)
{
    char inputUsername[MAX_USERNAME_LENGTH] = "";
    char inputPassword[MAX_PASSWORD_LENGTH] = "";
    int isAuthenticated = 0;
    int isUsernameFocused = 0;
    int isPasswordFocused = 0;
    int isLoginFailed = 0;

    InitWindow(1920, 1020,  "Login");
    SetTargetFPS(60);

    Texture2D background = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\svnlvn.jpg");

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    int textWidth = 180;
    int textHeight = 20;
    int boxWidth = 400;
    int boxHeight = 40;

    int usernameX = (screenWidth - textWidth) / 2;
    int passwordX = (screenWidth - textWidth) / 2;
    int usernameBoxX = (screenWidth - boxWidth) / 2;
    int passwordBoxX = (screenWidth - boxWidth) / 2;

    int usernameY = screenHeight / 2 - 60;
    int passwordY = screenHeight / 2 + 40;

    Rectangle usernameBox = { usernameBoxX, usernameY + textHeight + 5, boxWidth, boxHeight };
    Rectangle passwordBox = { passwordBoxX, passwordY + textHeight + 5, boxWidth, boxHeight };

    Rectangle exitButton = { screenWidth - 120, 20, 100, 40 };

     int verticalOffSet = 2;

    Rectangle loginButton = { (screenWidth - 200) / 2, screenHeight - 260 + verticalOffSet, 200, 40 };

    while (!WindowShouldClose() && !isAuthenticated)
    {
        Vector2 mousePosition = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(mousePosition, usernameBox))
            {
                isUsernameFocused = 1;
                isPasswordFocused = 0;
            }
            else if (CheckCollisionPointRec(mousePosition, passwordBox))
            {
                isPasswordFocused = 1;
                isUsernameFocused = 0;
            }
            else if (CheckCollisionPointRec(mousePosition, exitButton))
            {
                CloseWindow();
                return 0;
            }
            else if (CheckCollisionPointRec(mousePosition, loginButton))
            {
                isLoginFailed = 1;
                for (int i = 0; i < userCount; i++)
                {
                    if (strcmp(users[i].username, inputUsername) == 0 && strcmp(users[i].password, inputPassword) == 0)
                    {
                        isAuthenticated = 1;
                        isLoginFailed = 0;
                        break;
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        DrawScaledBackground(background, screenWidth, screenHeight);

        DrawText("Enter Username:", usernameX, usernameY, textHeight, WHITE);
        DrawText("Enter Password:", passwordX, passwordY, textHeight, WHITE);

        DrawRectangleLinesEx(usernameBox, 2, isUsernameFocused ? BLACK : WHITE);
        DrawRectangleLinesEx(passwordBox, 2, isPasswordFocused ? BLACK : WHITE);

        if (isUsernameFocused)
        {
            char keyPressed = GetCharPressed();
            while (keyPressed)
            {
                if (keyPressed != 0 && isalpha(keyPressed) && strlen(inputUsername) < MAX_USERNAME_LENGTH - 1)
                {
                    strncat(inputUsername, (char[]){keyPressed, '\0'}, 1);
                }
                keyPressed = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && strlen(inputUsername) > 0)
            {
                inputUsername[strlen(inputUsername) - 1] = '\0';
            }
        }

        if (isPasswordFocused)
        {
            char keyPressed = GetCharPressed();
            while (keyPressed)
            {
                if (keyPressed != 0 && isalpha(keyPressed) && strlen(inputPassword) < MAX_PASSWORD_LENGTH - 1)
                {
                    strncat(inputPassword, (char[]){keyPressed, '\0'}, 1);
                }
                keyPressed = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && strlen(inputPassword) > 0)
            {
                inputPassword[strlen(inputPassword) - 1] = '\0';
            }
        }

        DrawText(inputUsername, usernameBox.x + 10, usernameBox.y + 10, 20, WHITE);

        for (int i = 0; i < strlen(inputPassword); i++)
        {
            DrawText("*", passwordBox.x + 10 + (i * 20), passwordBox.y + 10, 20, WHITE);
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            isLoginFailed = 1;
            for (int i = 0; i < userCount; i++)
            {
                if (strcmp(users[i].username, inputUsername) == 0 && strcmp(users[i].password, inputPassword) == 0)
                {
                    isAuthenticated = 1;
                    isLoginFailed = 0;
                    break;
                }
            }
        }

        if (isLoginFailed)
        {
            DrawText("Invalid login, please try again.", 810, 650, 20, RED);
        }

        if (CheckCollisionPointRec(mousePosition, exitButton))
        {
            DrawRectangleRec(exitButton, RED);
        }
        else
        {
            DrawRectangleRec(exitButton, WHITE);
        }
        DrawText("EXIT", exitButton.x + 20, exitButton.y + 10, 20, BLACK);


        if (CheckCollisionPointRec(mousePosition, loginButton))
        {
            DrawRectangleRec(loginButton, LIME);
        }
        else
        {
            DrawRectangleRec(loginButton, DARKGRAY);
        }
        DrawText("LOGIN", loginButton.x + (loginButton.width - MeasureText("LOGIN", 20)) / 2, loginButton.y + (loginButton.height - 20) / 2, 20, WHITE);

        int bottomLoginY = screenHeight - 700;
        int loginX = (screenWidth - textWidth) / 2;

        int titleFontSize = 60;

        int verticalOffset = -5;

        DrawText("LOGIN", loginX, bottomLoginY, titleFontSize, WHITE);

        DrawText("LOGIN", loginX, bottomLoginY + verticalOffset, titleFontSize , LIME);

        EndDrawing();
    }

    UnloadTexture(background);

    CloseWindow();
    return isAuthenticated;
}

void CreateRecord()
{
    char inputText[MAX_USERNAME_LENGTH] = "";
    int letterCount = 0;
    bool recordCreated = false;

    Texture2D background = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\creation.jpg");

    if (recordCount >= MAX_RECORDS)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        const char *message = "Maximum record limit reached. Cannot create more records.";
        DrawText(message, GetScreenWidth() / 2 - MeasureText(message, 20) / 2, GetScreenHeight() / 2 - 10, 20, RED);
        EndDrawing();
        CustomWaitTime(2);
        UnloadTexture(background);
        return;
    }

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    while (!recordCreated && !WindowShouldClose())
        {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawScaledBackground(background, screenWidth, screenHeight);
        Vector2 mousePosition = GetMousePosition();

        if (DrawBackButton(20, 20, 80, 30)) {
            break;
        }

        DrawCenteredText("NEW RECORD", 25, 60, WHITE);
        DrawCenteredText("NEW RECORD", 20, 60, BLACK);

        int inputY = screenHeight / 2 - 60;
        DrawCenteredText("Enter the name for the new record:", inputY - 50, 20, BLACK);

        int inputBoxX = screenWidth / 2 - 200;
        DrawRectangle(inputBoxX, inputY, 400, 40, LIGHTGRAY);
        DrawText(inputText, inputBoxX + 10, inputY + 10, 20, BLACK);
        DrawRectangleLines(inputBoxX, inputY, 400, 40, DARKGRAY);

        int buttonWidth = 200;
        int buttonHeight = 40;
        Rectangle CreateButton = {
            (screenWidth - buttonWidth) / 2,
            inputY + 60,
            buttonWidth,
            buttonHeight
        };

        if (CheckCollisionPointRec(mousePosition, CreateButton))
        {
            DrawRectangleRec(CreateButton, LIME);
        } else
        {
            DrawRectangleRec(CreateButton, DARKGRAY);
        }
        DrawText("CREATE", CreateButton.x + (CreateButton.width - MeasureText("CREATE", 20)) / 2, CreateButton.y + (CreateButton.height - 20) / 2, 20, WHITE);

        if (letterCount < MAX_USERNAME_LENGTH - 1) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125)) {
                    inputText[letterCount] = (char)key;
                    letterCount++;
                    inputText[letterCount] = '\0';
                }
                key = GetCharPressed();
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0) {
            letterCount--;
            inputText[letterCount] = '\0';
        }

        bool createClicked = CheckCollisionPointRec(mousePosition, CreateButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

        if ((createClicked || IsKeyPressed(KEY_ENTER)) && letterCount > 0) {
            strncpy(records[recordCount].recordName, inputText, MAX_USERNAME_LENGTH);
            records[recordCount].recordName[MAX_USERNAME_LENGTH - 1] = '\0';
            records[recordCount].itemCount = 0;

            currentRecordIndex = recordCount;
            recordCount++;

            saveRecordsToFile("C:\\Users\\Rea Mae\\OneDrive\\Desktop\\recording.txt");

            recordCreated = true;
        }

        EndDrawing();
    }

    if (recordCreated)
    {
        Texture2D confirmBackground = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\creation2.jpg");
        while (!WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawScaledBackground(confirmBackground, screenWidth, screenHeight);

            const char *confirmationMsg = TextFormat("New record '%s' created and saved.", inputText);
            DrawText(confirmationMsg, screenWidth / 2 - MeasureText(confirmationMsg, 20) / 2, screenHeight / 2 - 30, 20, BLACK);
            DrawText("Press Enter key to continue...", screenWidth / 2 - MeasureText("Press Enter key to continue...", 20) / 2, screenHeight / 2 + 10, 20, BLACK);

            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                break;
            }

            EndDrawing();
        }
        UnloadTexture(confirmBackground);
    }

    UnloadTexture(background);
}


void initialQuantity(int index)
{
    Item *item = &records[currentRecordIndex].items[index];

    item->initialQuantity = item->quantity;

    Texture2D background = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\initial_quantity_recorded.jpg");
    DrawScaledBackground(background, GetScreenWidth(), GetScreenHeight());

    DrawCenteredText("Initial quantity recorded!", 30, 60, WHITE);
    DrawCenteredText("Initial quantity recorded!", 30, 60, BLACK);

    UnloadTexture(background);

    summaryRecord();
}

void addProd()
{
    char inputID[100] = {0}, inputName[100] = {0};
    char inputQtyStr[10] = {0}, inputPriceStr[20] = {0};
    int inputQty = 0;
    float inputPrice = 0.0;
    int activeField = 0; // 0 = ID, 1 = Name, 2 = Quantity, 3 = Price

    bool idTaken = false;
    bool nameTaken = false;

    // Load background texture
    Texture2D background = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\addProduct2.jpg");

    if (currentRecordIndex == -1)
    {
        ClearBackground(LIGHTGRAY);
        DrawScaledBackground(background, GetScreenWidth(), GetScreenHeight());
        showMessage("No active record. Please create or switch to a record first.");
        UnloadTexture(background);
        return;
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        DrawScaledBackground(background, GetScreenWidth(), GetScreenHeight());

        // Back Button
        if (DrawBackButton(20, 20, 80, 30))
        {
            EndDrawing();
            break; // Exit to main menu
        }

        DrawCenteredText("ADD PRODUCT", 25, 60, WHITE);
        DrawCenteredText("ADD PRODUCT", 20, 60, BLACK);

        // Render input fields
        DrawInputCentered("Enter Product ID:", inputID, 220, 50, BLACK, activeField == 0 ? BLACK : WHITE);
        if (idTaken)
            DrawText("ID already taken!", 885, 260, 20, RED);

        DrawInputCentered("Enter Product Name:", inputName, 300, 50, BLACK, activeField == 1 ? BLACK : WHITE);
        if (nameTaken)
            DrawText("Name already taken!", 885, 360, 20, RED);

        DrawInputCentered("Enter Quantity:", inputQtyStr, 390, 50, BLACK, activeField == 2 ? BLACK : WHITE);
        DrawInputCentered("Enter Price:", inputPriceStr, 480, 50, BLACK, activeField == 3 ? BLACK : WHITE);

        DrawText("Press ENTER KEY to Save.", 40, GetScreenHeight() - 147, 20, WHITE);
        DrawText("Press ENTER KEY to Save.", 40, GetScreenHeight() - 150, 20, BLACK);
        DrawText("Press BACK BUTTON to Cancel.", 40, GetScreenHeight() - 117, 20, WHITE);
        DrawText("Press BACK BUTTON to Cancel.", 40, GetScreenHeight() - 120, 20, BLACK);

        EndDrawing();

        if (IsKeyPressed(KEY_DOWN))
        {
            activeField = (activeField + 1) % 4;
            continue;
        }
        if (IsKeyPressed(KEY_UP))
        {
            activeField = (activeField - 1 + 4) % 4;
            continue;
        }

        if (activeField == 0)
            handleInput(inputID, sizeof(inputID));
        else if (activeField == 1)
            handleInput(inputName, sizeof(inputName));
        else if (activeField == 2)
            handleInput(inputQtyStr, sizeof(inputQtyStr));
        else if (activeField == 3)
            handleInput(inputPriceStr, sizeof(inputPriceStr));

        if (IsKeyPressed(KEY_ENTER))
        {
            inputQty = atoi(inputQtyStr);
            inputPrice = atof(inputPriceStr);

            // Validate inputs
            if (strlen(inputID) == 0 || strlen(inputName) == 0 || inputQty <= 0 || inputPrice <= 0.0f || !isdigit((unsigned char)inputID[0]))
            {
                showMessage("Please fill all fields correctly!");
                continue;
            }

            // Check for duplicate ID
            idTaken = false;
            StockRecord *currentRecord = &records[currentRecordIndex];
            for (int i = 0; i < currentRecord->itemCount; i++)
            {
                if (strcmp(currentRecord->items[i].id, inputID) == 0)
                {
                    idTaken = true;
                    break;
                }
            }

            // Check for duplicate name (case-insensitive)
            nameTaken = false;
            for (int i = 0; i < currentRecord->itemCount; i++)
            {
                if (caseInsensitiveCompare(currentRecord->items[i].name, inputName) == 0)
                {
                    nameTaken = true;
                    break;
                }
            }

            if (idTaken || nameTaken)
            {
                continue; // Ask user to reenter valid data
            }

            if (currentRecord->itemCount >= MAX_ITEMS)
            {
                showMessage("This record is full. Cannot add more products.");
                continue;
            }

            // Add new product
            Item newItem;
            strcpy(newItem.id, inputID);
            strcpy(newItem.name, inputName);
            newItem.quantity = inputQty;
            newItem.price = inputPrice;
            newItem.sales = 0.0f;
            newItem.numSold = 0;
            newItem.log[0] = '\0';
            newItem.initialQuantity = inputQty;

            currentRecord->items[currentRecord->itemCount++] = newItem;
            saveRecordsToFile("C:\\Users\\Rea Mae\\OneDrive\\Desktop\\recording.txt");

            showMessage("Product added successfully!");
            break;
        }
    }

    UnloadTexture(background);
}

void deleteProd(int index)
{
    if (currentRecordIndex == -1 || index < 0 || index >= records[currentRecordIndex].itemCount)
        return;

    Item *itemToDelete = &records[currentRecordIndex].items[index];
    printf("Product ID: %s, Product Name: %s has been deleted.\n", itemToDelete->id, itemToDelete->name);

    for (int i = index; i < records[currentRecordIndex].itemCount - 1; i++)
    {
        records[currentRecordIndex].items[i] = records[currentRecordIndex].items[i + 1];
    }
    records[currentRecordIndex].itemCount--;

    saveRecordsToFile("C:\\Users\\Rea Mae\\OneDrive\\Desktop\\recording.txt");
}

void renameProd(int index)
{
    if (currentRecordIndex == -1 || index < 0 || index >= records[currentRecordIndex].itemCount)
        return;

    char newName[100] = "\0";
    bool renaming = true;
    Texture2D background = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\newname.jpg");

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    int inputBoxWidth = 300;
    int inputBoxHeight = 45;
    int buttonWidth = 100;
    int buttonHeight = 30;
    int verticalSpacing = 20;
    while (renaming)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawScaledBackground(background, screenWidth, screenHeight);

        if (DrawBackButton(20, 20, 80, 30))
        {
            EndDrawing();
            break;
        }

        DrawCenteredText("NEW NAME", 23, 50, WHITE);
        DrawCenteredText("NEW NAME", 20, 50, BLACK);
        DrawText("Enter new product name:", centerX - MeasureText("Enter new product name:", 20) / 2, centerY - 100, 20, BLACK);

        handleTextInput(newName, sizeof(newName), centerX - inputBoxWidth / 2, centerY - 50, inputBoxWidth, inputBoxHeight, BLACK, WHITE);

        DrawTextEx(GetFontDefault(), TextFormat("Current Product: %s", records[currentRecordIndex].items[index].name),
                   (Vector2){centerX - MeasureText(TextFormat("Current Product: %s", records[currentRecordIndex].items[index].name), 20) / 2, centerY + 30}, 20, 2, BLACK);

        Rectangle renameButton = {centerX - buttonWidth / 2, centerY + 80, buttonWidth, buttonHeight};
        Color buttonColor = (CheckCollisionPointRec(GetMousePosition(), renameButton)) ? LIME : DARKGRAY;
        DrawRectangleRec(renameButton, buttonColor);
        DrawText("Rename", renameButton.x + (buttonWidth - MeasureText("Rename", 20)) / 2, renameButton.y + (buttonHeight - 20) / 2, 20, WHITE);

        Rectangle cancelButton = {centerX - buttonWidth / 2 + 110, centerY + 80, buttonWidth, buttonHeight};
        buttonColor = (CheckCollisionPointRec(GetMousePosition(), cancelButton)) ? RED : DARKGRAY;
        DrawRectangleRec(cancelButton, buttonColor);
        DrawText("Cancel", cancelButton.x + (buttonWidth - MeasureText("Cancel", 20)) / 2, cancelButton.y + (buttonHeight - 20) / 2, 20, WHITE);

        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(GetMousePosition(), renameButton))
            {
                if (strlen(newName) > 0)
                {
                    strncpy(records[currentRecordIndex].items[index].name, newName, sizeof(records[currentRecordIndex].items[index].name) - 1);
                    records[currentRecordIndex].items[index].name[sizeof(records[currentRecordIndex].items[index].name) - 1] = '\0';
                    saveRecordsToFile("C:\\Users\\Rea Mae\\OneDrive\\Desktop\\recording.txt");
                }
                renaming = false;
            }
            else if (CheckCollisionPointRec(GetMousePosition(), cancelButton))
            {
                renaming = false;
            }
        }
    }

    UnloadTexture(background);
}

void editPrice(int index)
{
    if (currentRecordIndex == -1 || index < 0 || index >= records[currentRecordIndex].itemCount)
        return;

    char newPriceStr[50] = "\0";
    bool editing = true;
    Texture2D background = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\newname.jpg");

    const char *productName = records[currentRecordIndex].items[index].name;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    int inputBoxWidth = 300;
    int inputBoxHeight = 45;
    int buttonWidth = 100;
    int buttonHeight = 30;
    int verticalSpacing = 20;

    while (editing)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawScaledBackground(background, screenWidth, screenHeight);

        if (DrawBackButton(20, 20, 80, 30))
        {
            EndDrawing();
            break;
        }

        DrawCenteredText("EDIT PRICE", 23, 50, WHITE);
        DrawCenteredText("EDIT PRICE", 20, 50, BLACK);

        DrawText("Editing Price for Product:", centerX - MeasureText("Editing Price for Product:", 20) / 2, centerY - 120, 20, BLACK);
        DrawText(productName, centerX - MeasureText(productName, 20) / 2, centerY - 90, 20, MAROON);

        DrawText("Enter new product price:", centerX - MeasureText("Enter new product price:", 20) / 2, centerY - 50, 20, BLACK);

        handleNumericInput(newPriceStr, sizeof(newPriceStr), centerX - inputBoxWidth / 2, centerY - 20, inputBoxWidth, inputBoxHeight, BLACK, WHITE);

        DrawTextEx(GetFontDefault(), TextFormat("Current Price: %.2f", records[currentRecordIndex].items[index].price),
                   (Vector2){centerX - MeasureText(TextFormat("Current Price: %.2f", records[currentRecordIndex].items[index].price), 20) / 2, centerY + 40}, 20, 2, GRAY);

        Rectangle updateButton = {centerX - buttonWidth / 2 - 60, centerY + 100, buttonWidth, buttonHeight};
        Color buttonColor = (CheckCollisionPointRec(GetMousePosition(), updateButton)) ? LIME : DARKGRAY;
        DrawRectangleRec(updateButton, buttonColor);
        DrawText("Update", updateButton.x + (buttonWidth - MeasureText("Update", 20)) / 2, updateButton.y + (buttonHeight - 20) / 2, 20, WHITE);

        Rectangle cancelButton = {centerX - buttonWidth / 2 + 60, centerY + 100, buttonWidth, buttonHeight};
        buttonColor = (CheckCollisionPointRec(GetMousePosition(), cancelButton)) ? RED : DARKGRAY;
        DrawRectangleRec(cancelButton, buttonColor);
        DrawText("Cancel", cancelButton.x + (buttonWidth - MeasureText("Cancel", 20)) / 2, cancelButton.y + (buttonHeight - 20) / 2, 20, WHITE);

        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(GetMousePosition(), updateButton))
            {
                float newPrice = atof(newPriceStr);
                if (newPrice > 0.0f)
                {
                    records[currentRecordIndex].items[index].price = newPrice;
                    saveRecordsToFile("C:\\Users\\Rea Mae\\OneDrive\\Desktop\\recording.txt");
                }
                editing = false;
            }
            else if (CheckCollisionPointRec(GetMousePosition(), cancelButton))
            {
                editing = false;
            }
        }
    }

    UnloadTexture(background);
}


void editID(int index)
{
    if (index < 0 || index >= records[currentRecordIndex].itemCount)
        return;

    char newID[50] = "\0";

    Texture2D background = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\editID.jpg");
    const char *productName = records[currentRecordIndex].items[index].name;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    int inputBoxWidth = 300;
    int inputBoxHeight = 40;
    int buttonWidth = 120;
    int buttonHeight = 40;
    int verticalSpacing = 20;

    Rectangle confirmButton = {centerX - buttonWidth / 2, centerY + 120, buttonWidth, buttonHeight};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawScaledBackground(background, screenWidth, screenHeight);

        if (DrawBackButton(20, 20, 80, 30))
        {
            EndDrawing();
            break;
        }

        DrawCenteredText("EDIT ID", 23, 50, WHITE);
        DrawCenteredText("EDIT ID", 20, 50, BLACK);
        DrawText("Edit ID for Product:", centerX - MeasureText("Edit ID for Product:", 30) / 2, centerY - 100, 30, BLACK);

        DrawText(productName, centerX - MeasureText(productName, 30) / 2, centerY - 60, 30, MAROON);

        DrawText("Enter the new Product ID:", centerX - MeasureText("Enter the new Product ID:", 20) / 2, centerY - 20, 20, BLACK);

        handleTextInput(newID, sizeof(newID), centerX - inputBoxWidth / 2, centerY + 50, inputBoxWidth, inputBoxHeight, BLACK, WHITE);

        bool isHovered = CheckCollisionPointRec(GetMousePosition(), confirmButton);

        if (isHovered)
        {
            DrawRectangleRec(confirmButton, DARKGREEN);
        }
        else
        {
            DrawRectangleRec(confirmButton, GREEN);
        }

        DrawText("CONFIRM", confirmButton.x + (buttonWidth - MeasureText("CONFIRM", 20)) / 2, confirmButton.y + (buttonHeight - 20) / 2, 20, WHITE);

        if (((isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_ENTER)) && strlen(newID) > 0)
        {
            Item *itemToEdit = &records[currentRecordIndex].items[index];
            strncpy(itemToEdit->id, newID, sizeof(itemToEdit->id) - 1);
            itemToEdit->id[sizeof(itemToEdit->id) - 1] = '\0';
            saveRecordsToFile("C:\\Users\\Rea Mae\\OneDrive\\Desktop\\recording.txt");
            EndDrawing();
            break;
        }

        EndDrawing();
    }

    UnloadTexture(background);
}

void purchaseProd(int index)
{
    Texture2D background = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\editID.jpg");
    const char *productName = records[currentRecordIndex].items[index].name;

    if (index < 0 || index >= records[currentRecordIndex].itemCount)
    {
        UnloadTexture(background);
        return;
    }

    bool showInvalidMessage = false;

    int localPurchaseQty = 0;
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    int offsetX = 190;
    int inputBoxWidth = 150;
    int inputBoxHeight = 40;
    int buttonWidth = 120;
    int buttonHeight = 40;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawScaledBackground(background, screenWidth, screenHeight);

        if (DrawBackButton(20, 20, 80, 30))
        {
            EndDrawing();
            break;
        }

        DrawCenteredText("PURCHASE PRODUCT", 23, 50, WHITE);
        DrawCenteredText("PURCHASE PRODUCT", 20, 50, BLACK);
        DrawText("Name of Product:", centerX - MeasureText("Name of Product:", 30) / 2, centerY - 100, 30, BLACK);

        DrawText(productName, centerX - MeasureText(productName, 30) / 2, centerY - 60, 30, MAROON);

        const char *quantityLabel = "Enter quantity to purchase:";
        DrawText(quantityLabel, centerX - MeasureText(quantityLabel, 20) / 2, centerY + 5, 20, BLACK);

        char stockText[50];
        sprintf(stockText, "Current stock: %d", records[currentRecordIndex].items[index].quantity);
        DrawText(stockText, centerX - MeasureText(stockText, 20) / 2, centerY - 15, 20, BLACK);

        DrawRectangle(centerX - inputBoxWidth / 2, centerY + 50, inputBoxWidth, inputBoxHeight, LIGHTGRAY);

        char buffer[20];
        sprintf(buffer, "%d", localPurchaseQty);
        DrawText(buffer, centerX - MeasureText(buffer, 20) / 2, centerY + 55, 20, BLACK);

        if (IsKeyPressed(KEY_BACKSPACE) && localPurchaseQty > 0) {
            localPurchaseQty = localPurchaseQty / 10;
        }

        int key = GetCharPressed();
        while (key > 0) {
            if (key >= '0' && key <= '9') {
                localPurchaseQty = localPurchaseQty * 10 + (key - '0');
            }
            key = GetCharPressed();
        }

        Rectangle confirmButton = {centerX - buttonWidth / 2, centerY + 110, buttonWidth, buttonHeight};

        bool isHovered = CheckCollisionPointRec(GetMousePosition(), confirmButton);

        if (isHovered) {
            DrawRectangleRec(confirmButton, DARKGREEN);
        } else {
            DrawRectangleRec(confirmButton, GREEN);
        }

        DrawText("CONFIRM",
                 confirmButton.x + (buttonWidth - MeasureText("CONFIRM", 20)) / 2,
                 confirmButton.y + (buttonHeight - 20) / 2, 20, WHITE);

        if ((isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_ENTER))
        {
            if (localPurchaseQty > 0 && localPurchaseQty <= records[currentRecordIndex].items[index].quantity)
            {
                Item *itemToPurchase = &records[currentRecordIndex].items[index];

                float totalSale = localPurchaseQty * itemToPurchase->price;
                itemToPurchase->sales += totalSale;
                itemToPurchase->numSold += localPurchaseQty;
                itemToPurchase->quantity -= localPurchaseQty;
                purchaseQuantity = localPurchaseQty;

                saveRecordsToFile("C:\\Users\\Rea Mae\\OneDrive\\Desktop\\recording.txt");
                EndDrawing();
                break;
            }
            else
            {
                showInvalidMessage = true;
            }
        }

        if (showInvalidMessage)
        {
            DrawText("Invalid quantity!", centerX - MeasureText("Invalid quantity!", 20) / 2, centerY + 155, 20, RED);
        }

        EndDrawing();
    }

    UnloadTexture(background);
}


void restockProd(int index)
{
    Texture2D background = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\editID.jpg");

    if (index < 0 || index >= records[currentRecordIndex].itemCount)
    {
        UnloadTexture(background);
        return;
    }

    int restockQuantity = 0;
    bool invalidQuantity = false;
    bool success = false;

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    int inputBoxWidth = 150;
    int inputBoxHeight = 40;
    int buttonWidth = 120;
    int buttonHeight = 40;
    int verticalSpacing = 20;

    Rectangle confirmButton = {centerX - buttonWidth / 2, centerY + 60, buttonWidth, buttonHeight};

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawScaledBackground(background, screenWidth, screenHeight);

        if (DrawBackButton(20, 20, 80, 30))
        {
            EndDrawing();
            break;
        }

        DrawCenteredText("RESTOCK PRODUCT", 23, 50, WHITE);
        DrawCenteredText("RESTOCK PRODUCT", 20, 50, BLACK);
        DrawText("Enter quantity to restock:", centerX - MeasureText("Enter quantity to restock:", 20) / 2, centerY - 100, 20, BLACK);
        DrawText(TextFormat("Current stock: %d", records[currentRecordIndex].items[index].quantity),
                 centerX - MeasureText(TextFormat("Current stock: %d", records[currentRecordIndex].items[index].quantity), 20) / 2,
                 centerY - 50, 20, BLACK);

        DrawRectangle(centerX - inputBoxWidth / 2, centerY, inputBoxWidth, inputBoxHeight, LIGHTGRAY);
        char buffer[20];
        sprintf(buffer, "%d", restockQuantity);
        DrawText(buffer, centerX - MeasureText(buffer, 20) / 2, centerY + 5, 20, BLACK);
        if (IsKeyPressed(KEY_BACKSPACE) && restockQuantity > 0)
        {
            restockQuantity = restockQuantity / 10;
        }

        int key = GetCharPressed();
        while (key > 0)
        {
            if (key >= '0' && key <= '9')
            {
                restockQuantity = restockQuantity * 10 + (key - '0');
            }
            key = GetCharPressed();
        }

        bool isHovered = CheckCollisionPointRec(GetMousePosition(), confirmButton);

        if (isHovered)
        {
            DrawRectangleRec(confirmButton, DARKGREEN);
        }
        else
        {
            DrawRectangleRec(confirmButton, GREEN);
        }

        DrawText("CONFIRM", confirmButton.x + (buttonWidth - MeasureText("CONFIRM", 20)) / 2, confirmButton.y + (buttonHeight - 20) / 2, 20, WHITE);

        if ((isHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_ENTER))
        {
            if (restockQuantity > 0)
            {
                records[currentRecordIndex].items[index].quantity += restockQuantity;
                saveRecordsToFile("C:\\Users\\Rea Mae\\OneDrive\\Desktop\\recording.txt");

                success = true;
                break;
            }
            else
            {
                invalidQuantity = true;
            }
        }

        if (invalidQuantity)
        {
            DrawText("Invalid quantity!", centerX - MeasureText("Invalid quantity!", 20) / 2, centerY + 120, 20, RED);
        }
        else if (success)
        {
            DrawText("Product restocked successfully!", centerX - MeasureText("Product restocked successfully!", 20) / 2, centerY + 120, 20, GREEN);
        }

        EndDrawing();
    }

    UnloadTexture(background);
}

void ViewCurrentRecord()
{
    Texture2D background = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\deletee.jpg");

    if (currentRecordIndex == -1 || recordCount == 0)
    {
        Texture2D noRecordBg = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\norecord.jpg");
        float waitTime = 2.0f;
        float elapsedTime = 0.0f;
        while (elapsedTime < waitTime)
        {
            elapsedTime += GetFrameTime();
            BeginDrawing();
            ClearBackground(LIGHTGRAY);
            DrawScaledBackground(noRecordBg, GetScreenWidth(), GetScreenHeight());
            DrawText("No records available to Display.", 250, 250, 50, RED);
            DrawText("Please Create a New Record First...", 250, 300, 50, RED);
            EndDrawing();
        }
        UnloadTexture(noRecordBg);
        UnloadTexture(background);
        return;
    }

    bool viewing = true;
    char searchQuery[100] = "\0";
    int selectedProductIndex = -1;
    bool isSearchBoxActive = false;
    int sortingOption = 0;
    int startIndex = 0;
    int maxVisible = 17;

    Item *items = records[currentRecordIndex].items;
    int *itemCount = &records[currentRecordIndex].itemCount;

    while (viewing)
    {
        Vector2 mousePosition = GetMousePosition();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawScaledBackground(background, GetScreenWidth(), GetScreenHeight());

        DrawCenteredText("CURRENT RECORD ITEMS", 23, 50, WHITE);
        DrawCenteredText("CURRENT RECORD ITEMS", 20, 50, BLACK);

        if (sortingOption == 0)
        {
            qsort(items, *itemCount, sizeof(Item), compareByName);
        }
        else if (sortingOption == 1)
        {
            qsort(items, *itemCount, sizeof(Item), compareByPrice);
        }

        Rectangle backButton = {20, 20, 80, 30};

        bool isHoveringBackButton = CheckCollisionPointRec(mousePosition, backButton);

        DrawRectangleRec(backButton, isHoveringBackButton ? GRAY : DARKGRAY);
        DrawRectangleLinesEx(backButton, 2, BLACK);
        DrawText("BACK", backButton.x + (backButton.width - MeasureText("BACK", 20)) / 2, backButton.y + (backButton.height - 20) / 2, 20, WHITE);

        if (isHoveringBackButton && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            viewing = false;
            EndDrawing();
            break;
        }

        DrawText("Enter Product ID or Name to Search: ", GetScreenWidth() - 450, 90, 20, BLACK);

        Rectangle searchBox = {GetScreenWidth() - 450, 110, 390, 40};

        DrawRectangleRec(searchBox, isSearchBoxActive ? LIGHTGRAY : WHITE);
        DrawRectangleLinesEx(searchBox, 2, BLACK);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(mousePosition, searchBox))
            {
                isSearchBoxActive = true;
            }
          else
            {
                isSearchBoxActive = false;
            }
        }

        if (isSearchBoxActive)
        {
            handleTextInput(searchQuery, sizeof(searchQuery), searchBox.x, searchBox.y, searchBox.width, searchBox.height, BLACK, WHITE);
        }
        else
        {
            DrawText(searchQuery, searchBox.x + 10, searchBox.y + 10, 20, BLACK);
        }

        int tableStartY = GetScreenHeight() / 2 - 320;
        int yOffset = tableStartY + 50;

        DrawRectangleRounded((Rectangle){50, tableStartY, GetScreenWidth() - 100, 40}, 0.2f, 10, LIGHTGRAY);
        DrawRectangleRounded((Rectangle){50, tableStartY, GetScreenWidth() - 100, 40}, 0.2f, 10, BLACK);

        int colWidths[] = {100, 800, 1200, 1900, 2450};
        int colStarts[] = {50, 150, 350, 450, 550};

        const char *headers[] = {"ID", "Product Name", "Quantity", "Price", "Sales"};
        for (int c = 0; c < 5; c++)
        {
            int textWidth = MeasureText(headers[c], 20);
            DrawText(headers[c], colStarts[c] + (colWidths[c] - textWidth) / 2, tableStartY + 10, 20, WHITE);
        }
        DrawLine(50, tableStartY + 40, GetScreenWidth() - 50, tableStartY + 40, BLACK);

        for (int i = startIndex; i < startIndex + maxVisible && i < *itemCount; i++)
        {
            Item item = items[i];

            if (strlen(searchQuery) > 0 && !strstr(item.name, searchQuery) && !strstr(item.id, searchQuery))
                continue;

            int yPos = yOffset + (i - startIndex) * 40;

            Rectangle itemRow = {50, yPos, GetScreenWidth() - 100, 40};

            if (CheckCollisionPointRec(mousePosition, itemRow))
            {
                DrawRectangleRounded(itemRow, 0.2f, 10, GRAY);
            }
            else
            {
                DrawRectangleRounded(itemRow, 0.2f, 10, i % 2 == 0 ? LIGHTGRAY : WHITE);
            }

            const char *rowData[] = {
                item.id,
                item.name,
                TextFormat("%d", item.quantity),
                TextFormat("%.2f", item.price),
                TextFormat("%.2f", item.sales)};

            for (int c = 0; c < 5; c++)
            {
                int textWidth = MeasureText(rowData[c], 20);
                DrawText(rowData[c], colStarts[c] + (colWidths[c] - textWidth) / 2, yPos + 10, 20, BLACK);
            }

            DrawLine(50, yPos + 40, GetScreenWidth() - 50, yPos + 40, BLACK);

            if (CheckCollisionPointRec(mousePosition, itemRow) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                selectedProductIndex = i;
            }
        }

        int buttonY = GetScreenHeight() - 80;

        float scrollAmount = GetMouseWheelMove();
        if (scrollAmount > 0 && startIndex > 0)
        {
            startIndex--;
        }
        else if (scrollAmount < 0 && startIndex + maxVisible < *itemCount)
        {
            startIndex++;
        }


        Rectangle addItemButton = {50, buttonY, 150, 30};
        Rectangle sortByNameButton = {390, buttonY, 150, 30};
        Rectangle sortByPriceButton = {220, buttonY, 150, 30};
        bool isHoveringAddItem = CheckCollisionPointRec(mousePosition, addItemButton);
        bool isHoveringSortByName = CheckCollisionPointRec(mousePosition, sortByNameButton);
        bool isHoveringSortByPrice = CheckCollisionPointRec(mousePosition, sortByPriceButton);

        DrawRectangleRec(addItemButton, isHoveringAddItem ? GRAY : DARKGRAY);
        DrawRectangleLinesEx(addItemButton, 2, BLACK);
        DrawText("Add Item", addItemButton.x + (addItemButton.width - MeasureText("Add Item", 20)) / 2, addItemButton.y + (addItemButton.height - 20) / 2, 20, WHITE);

        DrawRectangleRec(sortByNameButton, isHoveringSortByName ? GRAY : DARKGRAY);
        DrawRectangleLinesEx(sortByNameButton, 2, BLACK);
        DrawText("Sort by Name", sortByNameButton.x + (sortByNameButton.width - MeasureText("Sort by Name", 20)) / 2, sortByNameButton.y + (sortByNameButton.height - 20) / 2, 20, WHITE);

        DrawRectangleRec(sortByPriceButton, isHoveringSortByPrice ? GRAY : DARKGRAY);
        DrawRectangleLinesEx(sortByPriceButton, 2, BLACK);
        DrawText("Sort by Price", sortByPriceButton.x + (sortByPriceButton.width - MeasureText("Sort by Price", 20)) / 2, sortByPriceButton.y + (sortByPriceButton.height - 20) / 2, 20, WHITE);

        if (CheckCollisionPointRec(mousePosition, sortByNameButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            sortingOption = 0;
        }
        else if (CheckCollisionPointRec(mousePosition, sortByPriceButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            sortingOption = 1;
        }

        bool addItemClicked = CheckCollisionPointRec(mousePosition, addItemButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        if (selectedProductIndex != -1)
        {
            Rectangle deleteItemButton = {1240, buttonY, 150, 30};
            Rectangle renameItemButton = {1070, buttonY, 150, 30};
            Rectangle editIDButton = {560, buttonY, 150, 30};
            Rectangle purchaseItemButton = {730, buttonY, 150, 30};
            Rectangle restockItemButton = {900, buttonY, 150, 30};
            Rectangle editPriceButton = {1410, buttonY, 150, 30};

            bool isHoveringDelete = CheckCollisionPointRec(mousePosition, deleteItemButton);
            bool isHoveringRename = CheckCollisionPointRec(mousePosition, renameItemButton);
            bool isHoveringEditID = CheckCollisionPointRec(mousePosition, editIDButton);
            bool isHoveringPurchase = CheckCollisionPointRec(mousePosition, purchaseItemButton);
            bool isHoveringRestock = CheckCollisionPointRec(mousePosition, restockItemButton);
            bool isHoveringEditPrice = CheckCollisionPointRec(mousePosition, editPriceButton);

            DrawRectangleRec(deleteItemButton, isHoveringDelete ? GRAY : DARKGRAY);
            DrawRectangleLinesEx(deleteItemButton, 2, BLACK);
            DrawText("Delete Item", deleteItemButton.x + (deleteItemButton.width - MeasureText("Delete Item", 20)) / 2, deleteItemButton.y + (deleteItemButton.height - 20) / 2, 20, WHITE);

            DrawRectangleRec(renameItemButton, isHoveringRename ? GRAY : DARKGRAY);
            DrawRectangleLinesEx(renameItemButton, 2, BLACK);
            DrawText("Rename Item", renameItemButton.x + (renameItemButton.width - MeasureText("Rename Item", 20)) / 2, renameItemButton.y + (renameItemButton.height - 20) / 2, 20, WHITE);

            DrawRectangleRec(editIDButton, isHoveringEditID ? GRAY : DARKGRAY);
            DrawRectangleLinesEx(editIDButton, 2, BLACK);
            DrawText("Edit ID", editIDButton.x + (editIDButton.width - MeasureText("Edit ID", 20)) / 2, editIDButton.y + (editIDButton.height - 20) / 2, 20, WHITE);

            DrawRectangleRec(purchaseItemButton, isHoveringPurchase ? GRAY : DARKGRAY);
            DrawRectangleLinesEx(purchaseItemButton, 2, BLACK);
            DrawText("Purchase", purchaseItemButton.x + (purchaseItemButton.width - MeasureText("Purchase", 20)) / 2, purchaseItemButton.y + (purchaseItemButton.height - 20) / 2, 20, WHITE);

            DrawRectangleRec(restockItemButton, isHoveringRestock ? GRAY : DARKGRAY);
            DrawRectangleLinesEx(restockItemButton, 2, BLACK);
            DrawText("Restock", restockItemButton.x + (restockItemButton.width - MeasureText("Restock", 20)) / 2, restockItemButton.y + (restockItemButton.height - 20) / 2, 20, WHITE);

            DrawRectangleRec(editPriceButton, isHoveringEditPrice ? GRAY : DARKGRAY);
            DrawRectangleLinesEx(editPriceButton, 2, BLACK);
            DrawText("Edit Price", editPriceButton.x + (editPriceButton.width - MeasureText("Edit Price", 20)) / 2, editPriceButton.y + (editPriceButton.height - 20) / 2, 20, WHITE);

            bool deleteClicked = CheckCollisionPointRec(mousePosition, deleteItemButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
            bool renameClicked = CheckCollisionPointRec(mousePosition, renameItemButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
            bool editIDClicked = CheckCollisionPointRec(mousePosition, editIDButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
            bool purchaseClicked = CheckCollisionPointRec(mousePosition, purchaseItemButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
            bool restockClicked = CheckCollisionPointRec(mousePosition, restockItemButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
            bool editPriceClicked = CheckCollisionPointRec(mousePosition, editPriceButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

            EndDrawing();

            if (deleteClicked)
            {
                deleteProd(selectedProductIndex);
                selectedProductIndex = -1;
            }
            else if (renameClicked)
            {
                renameProd(selectedProductIndex);
            }
            else if (editIDClicked)
            {
                editID(selectedProductIndex);
            }
            else if (purchaseClicked)
            {
                purchaseProd(selectedProductIndex);
            }
            else if (restockClicked)
            {
                restockProd(selectedProductIndex);
            }
            else if (editPriceClicked)
            {
                editPrice(selectedProductIndex);
            }
        }
        else
        {
            EndDrawing();
            if (addItemClicked)
            {
                addProd();
            }
        }
    }

    UnloadTexture(background);
}

void renameRecord(int recordIndex)
{
    if (recordIndex < 0 || recordIndex >= recordCount)
    {
        return;
    }

    char newName[MAX_USERNAME_LENGTH] = "\0";
    bool renaming = true;
    Texture2D background = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\editID.jpg");  // Update with the correct path

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    int inputBoxWidth = 300;
    int inputBoxHeight = 45;
    int buttonWidth = 100;
    int buttonHeight = 30;
    int verticalSpacing = 20;

    while (renaming)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawScaledBackground(background, screenWidth, screenHeight);  // Draw the background

        if (DrawBackButton(20, 20, 80, 30))
        {
            EndDrawing();
            break;
        }

        DrawCenteredText("RENAME RECORD", 23, 50, WHITE);  // Title
        DrawCenteredText("RENAME RECORD", 20, 50, BLACK);  // Title shadow
        DrawText("Enter new record name:", centerX - MeasureText("Enter new record name:", 20) / 2, centerY - 100, 20, BLACK);

        handleTextInput(newName, sizeof(newName), centerX - inputBoxWidth / 2, centerY - 50, inputBoxWidth, inputBoxHeight, BLACK, WHITE);

        DrawTextEx(GetFontDefault(), TextFormat("Current Record: %s", records[recordIndex].recordName),
                   (Vector2){centerX - MeasureText(TextFormat("Current Record: %s", records[recordIndex].recordName), 20) / 2, centerY + 30}, 20, 2, BLACK);

        Rectangle renameButton = {centerX - buttonWidth / 2, centerY + 80, buttonWidth, buttonHeight};
        Color buttonColor = (CheckCollisionPointRec(GetMousePosition(), renameButton)) ? LIME : DARKGRAY;
        DrawRectangleRec(renameButton, buttonColor);
        DrawText("Rename", renameButton.x + (buttonWidth - MeasureText("Rename", 20)) / 2, renameButton.y + (buttonHeight - 20) / 2, 20, WHITE);

        Rectangle cancelButton = {centerX - buttonWidth / 2 + 110, centerY + 80, buttonWidth, buttonHeight};
        buttonColor = (CheckCollisionPointRec(GetMousePosition(), cancelButton)) ? RED : DARKGRAY;
        DrawRectangleRec(cancelButton, buttonColor);
        DrawText("Cancel", cancelButton.x + (buttonWidth - MeasureText("Cancel", 20)) / 2, cancelButton.y + (buttonHeight - 20) / 2, 20, WHITE);

        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(GetMousePosition(), renameButton))
            {
                if (strlen(newName) > 0)
                {
                    strncpy(records[recordIndex].recordName, newName, MAX_USERNAME_LENGTH - 1);
                    records[recordIndex].recordName[MAX_USERNAME_LENGTH - 1] = '\0';
                    saveRecordsToFile("C:\\Users\\Rea Mae\\OneDrive\\Desktop\\recording.txt");
                }
                renaming = false;
            }
            else if (CheckCollisionPointRec(GetMousePosition(), cancelButton))
            {
                renaming = false;
            }
        }
    }

    UnloadTexture(background);
}

void SwitchRecord()
{
    bool switching = true;
    bool recordSwitched = false;
    char searchQuery[100] = "\0";
    int startIndex = 0;
    int maxVisible = 20;

    Texture2D background = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\switchrec.jpg");

    if (recordCount == 0)
    {
        Texture2D noRecordBg = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\norecord.jpg");
        float waitTime = 2.0f;
        float elapsedTime = 0.0f;
        while (elapsedTime < waitTime)
        {
            elapsedTime += GetFrameTime();
            BeginDrawing();
            ClearBackground(LIGHTGRAY);
            DrawScaledBackground(noRecordBg, GetScreenWidth(), GetScreenHeight());
            DrawText("No records available to switch.", 250, 250, 50, RED);
            DrawText("Please Create a New Record First...", 250, 300, 50, RED);
            EndDrawing();
        }
        UnloadTexture(noRecordBg);
        UnloadTexture(background);
        return;
    }

    while (switching)
    {
        Vector2 mousePosition = GetMousePosition();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawScaledBackground(background, GetScreenWidth(), GetScreenHeight());

        if (DrawBackButton(20, 20, 80, 30))
        {
            switching = false;
            recordSwitched = false;
            EndDrawing();
            break;
        }

        DrawCenteredText("AVAILABLE RECORDS TO SWITCH", 23, 30, WHITE);
        DrawCenteredText("AVAILABLE RECORDS TO SWITCH", 20, 30, BLACK);

        DrawRectangle(GetScreenWidth() - 310, 20, 300, 40, LIGHTGRAY);
        DrawRectangleLines(GetScreenWidth() - 310, 20, 300, 40, BLACK);
        DrawText("Search:", GetScreenWidth() - 305, 25, 20, BLACK);
        handleTextInput(searchQuery, sizeof(searchQuery), GetScreenWidth() - 210, 25, 200, 30, BLACK, WHITE);

        DrawRectangleRounded((Rectangle){ 50, 80, GetScreenWidth() - 100, 60 }, 0.2f, 10, LIGHTGRAY);
        DrawRectangleRounded((Rectangle){ 50, 80, GetScreenWidth() - 100, 60 }, 0.2f, 10, BLACK);
        DrawTextEx(GetFontDefault(), "Record No.", (Vector2){ 55, 95 }, 20, 2, WHITE);
        DrawTextEx(GetFontDefault(), "Record Name", (Vector2){ 250, 95 }, 20, 2, WHITE);
        DrawLine(50, 120, GetScreenWidth() - 50, 120, BLACK);

        int clickedRecord = -1;

        for (int i = 0; i < maxVisible; i++)
        {
            int recordIndex = startIndex + i;
            if (recordIndex >= recordCount) break;

            if (strlen(searchQuery) > 0 && strstr(records[recordIndex].recordName, searchQuery) == NULL)
                continue;

            int yPos = 130 + i * 40;

            Color rowColor = (CheckCollisionPointRec(mousePosition, (Rectangle){ 50, yPos, GetScreenWidth() - 100, 40 })) ? GRAY : (i % 2 == 0 ? LIGHTGRAY : WHITE);
            DrawRectangleRounded((Rectangle){ 50, yPos, GetScreenWidth() - 100, 40 }, 0.2f, 10, rowColor);

            DrawTextEx(GetFontDefault(), TextFormat("%d", recordIndex + 1), (Vector2){ 55, yPos + 10 }, 20, 2, BLACK);
            DrawTextEx(GetFontDefault(), records[recordIndex].recordName, (Vector2){ 250, yPos + 10 }, 20, 2, BLACK);

            Rectangle recordRect = { 50, yPos, GetScreenWidth() - 100, 40 };
            if (CheckCollisionPointRec(mousePosition, recordRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                clickedRecord = recordIndex;
            }
        }

        float scrollAmount = GetMouseWheelMove();
        if (scrollAmount > 0 && startIndex > 0) startIndex--;
        else if (scrollAmount < 0 && startIndex + maxVisible < recordCount) startIndex++;

        Rectangle renameButton = {GetScreenWidth() / 2 - 100, GetScreenHeight() - 80, 200, 40};
        Color renameButtonColor = (CheckCollisionPointRec(mousePosition, renameButton)) ? RED : LIGHTGRAY;
        DrawRectangleRec(renameButton, renameButtonColor);
        DrawText("Rename Record", renameButton.x + 20, renameButton.y + 10, 20, BLACK);

        bool renameRecordClicked = CheckCollisionPointRec(mousePosition, renameButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        EndDrawing();

        if (clickedRecord != -1)
        {
            currentRecordIndex = clickedRecord;
            recordSwitched = true;
            switching = false;
            break;
        }

        if (renameRecordClicked && currentRecordIndex >= 0)
        {
            renameRecord(currentRecordIndex);
        }
    }

    if (recordSwitched)
    {
        Texture2D confirmBg = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\creation2.jpg");
        float waitTime = 1.0f;
        float elapsedTime = 0.0f;
        while (elapsedTime < waitTime)
        {
            elapsedTime += GetFrameTime();
            BeginDrawing();
            ClearBackground(LIGHTGRAY);
            DrawScaledBackground(confirmBg, GetScreenWidth(), GetScreenHeight());
            DrawText(TextFormat("Switched to record '%s'.", records[currentRecordIndex].recordName), 820, 500, 20, BLACK);
            EndDrawing();
        }
        UnloadTexture(confirmBg);
    }

    UnloadTexture(background);
}

void DeleteRecord() {
    bool deleting = true;
    bool recordDeleted = false;
    char searchQuery[100] = "\0";
    int startIndex = 0;
    int maxVisible = 20;

    Texture2D background = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\displayy.jpg");

    if (recordCount == 0)
    {
        Texture2D noRecordBg = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\norecord.jpg");
        float waitTime = 2.0f;
        float elapsedTime = 0.0f;
        while (elapsedTime < waitTime)
        {
            elapsedTime += GetFrameTime();
            BeginDrawing();
            ClearBackground(LIGHTGRAY);
            DrawScaledBackground(noRecordBg, GetScreenWidth(), GetScreenHeight());
            DrawText("No records available to delete.", 250, 250, 50, RED);
            DrawText("Please Create a New Record First...", 250, 300, 50, RED);
            EndDrawing();
        }
        UnloadTexture(noRecordBg);
        UnloadTexture(background);
        return;
    }

    while (deleting) {
        Vector2 mousePosition = GetMousePosition();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawScaledBackground(background, GetScreenWidth(), GetScreenHeight());

        if (DrawBackButton(20, 20, 80, 30))
        {
            deleting = false;
            recordDeleted = false;
            EndDrawing();
            break;
        }

        DrawCenteredText("AVAILABLE RECORDS TO DELETE", 23, 30, WHITE);
        DrawCenteredText("AVAILABLE RECORDS TO DELETE", 20, 30, BLACK);

        DrawRectangle(GetScreenWidth() - 310, 20, 300, 40, LIGHTGRAY);
        DrawRectangleLines(GetScreenWidth() - 310, 20, 300, 40, BLACK);
        DrawText("Search:", GetScreenWidth() - 305, 25, 20, BLACK);
        handleTextInput(searchQuery, sizeof(searchQuery), GetScreenWidth() - 210, 25, 200, 30, BLACK, WHITE);

        DrawRectangleRounded((Rectangle){ 50, 80, GetScreenWidth() - 100, 60 }, 0.2f, 10, LIGHTGRAY);
        DrawRectangleRounded((Rectangle){ 50, 80, GetScreenWidth() - 100, 60 }, 0.2f, 10, BLACK);
        DrawTextEx(GetFontDefault(), "Record No.", (Vector2){ 55, 95 }, 20, 2, WHITE);
        DrawTextEx(GetFontDefault(), "Record Name", (Vector2){ 250, 95 }, 20, 2, WHITE);
        DrawLine(50, 120, GetScreenWidth() - 50, 120, BLACK);

        int recordToDelete = -1;

        for (int i = 0; i < maxVisible; i++)
            {
            int recordIndex = startIndex + i;
            if (recordIndex >= recordCount) break;

            if (strlen(searchQuery) > 0 && strstr(records[recordIndex].recordName, searchQuery) == NULL)
                continue;

            int yPos = 130 + i * 40;

            Color rowColor = (CheckCollisionPointRec(mousePosition, (Rectangle){ 50, yPos, GetScreenWidth() - 100, 40 })) ? GRAY : (i % 2 == 0 ? LIGHTGRAY : WHITE);
            DrawRectangleRounded((Rectangle){ 50, yPos, GetScreenWidth() - 100, 40 }, 0.2f, 10, rowColor);

            DrawTextEx(GetFontDefault(), TextFormat("%d", recordIndex + 1), (Vector2){ 55, yPos + 10 }, 20, 2, BLACK);
            DrawTextEx(GetFontDefault(), records[recordIndex].recordName, (Vector2){ 250, yPos + 10 }, 20, 2, BLACK);

            Rectangle recordRect = { 50, yPos, GetScreenWidth() - 100, 40 };
            if (CheckCollisionPointRec(mousePosition, recordRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                recordToDelete = recordIndex;
            }
        }

        float scrollAmount = GetMouseWheelMove();
        if (scrollAmount > 0 && startIndex > 0) startIndex--;
        else if (scrollAmount < 0 && startIndex + maxVisible < recordCount) startIndex++;

        EndDrawing();

        if (recordToDelete != -1)
        {
            for (int j = recordToDelete; j < recordCount - 1; j++)
            {
                records[j] = records[j + 1];
            }
            recordCount--;
            if (currentRecordIndex == recordToDelete)
                currentRecordIndex = -1;
            else if (currentRecordIndex > recordToDelete)
                currentRecordIndex--;

            recordDeleted = true;
            saveRecordsToFile("C:\\Users\\Rea Mae\\OneDrive\\Desktop\\recording.txt");
            deleting = false;
        }
    }

    if (recordDeleted)
    {
        Texture2D confirmBg = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\creation2.jpg");
        float waitTime = 1.0f;
        float elapsedTime = 0.0f;
        while (elapsedTime < waitTime)
        {
            elapsedTime += GetFrameTime();
            BeginDrawing();
            ClearBackground(LIGHTGRAY);
            DrawScaledBackground(confirmBg, GetScreenWidth(), GetScreenHeight());
            DrawText("Record deleted successfully.", 820, 500, 20, BLACK);
            EndDrawing();
        }
        UnloadTexture(confirmBg);
    }

    UnloadTexture(background);
}

void saveSummaryToFile(StockRecord *currentRecord)
{
    // File path (you can change this to your desired location)
    const char *filePath = "C:\\Users\\Rea Mae\\OneDrive\\Desktop\\summary.txt";
    FILE *file = fopen(filePath, "w");


    if (file == NULL)
    {
        showMessage("Failed to open file for saving the summary.");
        return;
    }

    // Print headers to the file
    fprintf(file, "TRANSACTION SUMMARY\n");
    fprintf(file, "=====================\n");
    fprintf(file, "Item ID  | Name                  | Price   | Logs                     | Quantity         | Sales\n");
    fprintf(file, "------------------------------------------------------------------------------------------------\n");

    // Iterate through each item and save the details to the file
    for (int i = 0; i < currentRecord->itemCount; i++)
    {
        Item *item = &currentRecord->items[i];

        fprintf(file, "%-8s | %-20s | %-7.2f | %-23s | %-14d | %.2f\n",
        // Format and print each item's details
                item->id,
                item->name,
                item->price,
                (item->log[0] != '\0') ? item->log : "No changes",
                item->quantity,
                item->sales);
    }

    fprintf(file, "\nEnd of Summary\n");

    // Close the file after writing
    fclose(file);
}

void summaryRecord()
{
    static bool summarySaved = false;  // Flag to track if the summary is saved

    if (recordCount == 0) {
        // Display "No records to delete" message
        Texture2D noRecordBg = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\norecord.jpg");
        float waitTime = 2.0f; // 2-second delay
        float elapsedTime = 0.0f;
        while (elapsedTime < waitTime) {
            elapsedTime += GetFrameTime();

            BeginDrawing();
            ClearBackground(LIGHTGRAY);
            DrawScaledBackground(noRecordBg, GetScreenWidth(), GetScreenHeight());
            DrawText("No records available to delete.", 250, 250, 50, RED);
            DrawText("Please Create a New Record First...", 250, 300, 50, RED);
            EndDrawing();
        }
        UnloadTexture(noRecordBg);
        return;
    }

    StockRecord *currentRecord = &records[currentRecordIndex];
    Texture2D background = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\summarydis.jpg");
    Texture2D savedBackground = LoadTexture("C:\\Users\\Rea Mae\\Downloads\\creation2.jpg");

    int startIndex = 0;    // The starting index for the visible products
    int maxVisible = 18;

    // Calculate the screen dimensions
    int screenWidth = 1920;
    int screenHeight = 1020;

    Item *items = records[currentRecordIndex].items;
    int *itemCount = &records[currentRecordIndex].itemCount;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        // Back button (upper-left corner)
        if (DrawBackButton(20, 20, 80, 30)) {
            EndDrawing();
            break; // Return to the main menu
        }

        if (!summarySaved) {
            // Draw background
            DrawScaledBackground(background, GetScreenWidth(), GetScreenHeight());

            // Title
            DrawCenteredText("TRANSACTION SUMMARY", 23, 30, WHITE);
            DrawCenteredText("TRANSACTION SUMMARY", 20, 30, BLACK);

            // Table headers
            int tableStartX = 50;
            int tableStartY = 120;
            int headerHeight = 50;

            // Calculate column spacing for equal distribution
            int columnCount = 6; // Total number of columns
            float columnSpacing = (screenWidth - 100) / columnCount; // Equal spacing

            // Draw table headers
            DrawRectangleRounded(
                (Rectangle){tableStartX, tableStartY, screenWidth - 100, headerHeight}, 0.2f, 10, GRAY);
            DrawText("ID", tableStartX + columnSpacing / 2 - MeasureText("ID", 20) / 2, tableStartY + 15, 20, WHITE);
            DrawText("Name", tableStartX + columnSpacing + columnSpacing / 2 - MeasureText("Name", 20) / 2, tableStartY + 15, 20, WHITE);
            DrawText("Price", tableStartX + 2 * columnSpacing + columnSpacing / 2 - MeasureText("Price", 20) / 2, tableStartY + 15, 20, WHITE);
            DrawText("Logs", tableStartX + 3 * columnSpacing + columnSpacing / 2 - MeasureText("Logs", 20) / 2, tableStartY + 15, 20, WHITE);
            DrawText("Quantity", tableStartX + 4 * columnSpacing + columnSpacing / 2 - MeasureText("Quantity", 20) / 2, tableStartY + 15, 20, WHITE);
            DrawText("Sales", tableStartX + 5 * columnSpacing + columnSpacing / 2 - MeasureText("Sales", 20) / 2, tableStartY + 15, 20, WHITE);

            int rowHeight = 50;

            // Draw table content
            for (int index = 0; index < maxVisible && (index + startIndex) < *itemCount; index++) {
                int itemIndex = index + startIndex; // Calculate the actual item index
                int yPos = tableStartY + headerHeight + index * rowHeight;

                // Alternate row colors
                Color rowColor = (itemIndex % 2 == 0) ? LIGHTGRAY : WHITE;
                DrawRectangle(tableStartX, yPos, screenWidth - 100, rowHeight, rowColor);

                // Product details
                DrawText(TextFormat("%s", items[itemIndex].id),
                         tableStartX + columnSpacing / 2 - MeasureText(TextFormat("%s", items[itemIndex].id), 20) / 2,
                         yPos + 10, 20, BLACK);

                DrawText(items[itemIndex].name,
                         tableStartX + columnSpacing + columnSpacing / 2 - MeasureText(items[itemIndex].name, 20) / 2,
                         yPos + 10, 20, BLACK);

                DrawText(TextFormat("%.2f", items[itemIndex].price),
                         tableStartX + 2 * columnSpacing + columnSpacing / 2 - MeasureText(TextFormat("%.2f", items[itemIndex].price), 20) / 2,
                         yPos + 10, 20, BLACK);

                // Display logs
                if (strlen(items[itemIndex].log) > 0) {
                    DrawText(items[itemIndex].log,
                             tableStartX + 3 * columnSpacing + columnSpacing / 2 - MeasureText(items[itemIndex].log, 20) / 2,
                             yPos + 10, 20, GRAY);
                } else {
                    DrawText("No changes",
                             tableStartX + 3 * columnSpacing + columnSpacing / 2 - MeasureText("No changes", 20) / 2,
                             yPos + 10, 20, GRAY);
                }

                // Final Quantity and Sales
                DrawText(TextFormat("%d", items[itemIndex].quantity),
                         tableStartX + 4 * columnSpacing + columnSpacing / 2 - MeasureText(TextFormat("%d", items[itemIndex].quantity), 20) / 2,
                         yPos + 10, 20, BLACK);

                DrawText(TextFormat("%.2f", items[itemIndex].sales),
                         tableStartX + 5 * columnSpacing + columnSpacing / 2 - MeasureText(TextFormat("%.2f", items[itemIndex].sales), 20) / 2,
                         yPos + 10, 20, BLACK);
            }
            // Save button
            Rectangle saveButton = {screenWidth / 2 - 100, screenHeight - 100, 200, 50};

            // Check if the mouse is hovering over the save button
            bool isHovered = CheckCollisionPointRec(GetMousePosition(), saveButton);

            // Highlight the button on hover
            if (isHovered) {
                DrawRectangleRec(saveButton, GREEN); // Brighter green when hovered
            } else {
                DrawRectangleRec(saveButton, DARKGREEN); // Default color
            }

            DrawText("SAVE SUMMARY", saveButton.x + 20, saveButton.y + 15, 20, WHITE);

            if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                saveSummaryToFile(currentRecord);
                summarySaved = true;  // Set the flag to indicate the summary is saved
            }

            // Scroll functionality
            float scrollAmount = GetMouseWheelMove(); // Get mouse scroll movement
            if (scrollAmount > 0 && startIndex > 0) { // Scroll up
                startIndex--;
            } else if (scrollAmount < 0 && startIndex + maxVisible < *itemCount) { // Scroll down
                startIndex++;
            }
        } else {
            DrawScaledBackground(savedBackground, GetScreenWidth(), GetScreenHeight());

            DrawText("Summary saved to file!", screenWidth / 2 - 150, screenHeight / 2 - 50, 30, BLACK);
            DrawText("Press 'Enter' to return to summary", screenWidth / 2 - 180, screenHeight / 2, 20, BLACK);

            if (IsKeyPressed(KEY_ENTER)) {
                summarySaved = false; // Reset the flag to return to summary screen
            }
        }

        EndDrawing();
    }

    UnloadTexture(background);
    UnloadTexture(savedBackground);
}