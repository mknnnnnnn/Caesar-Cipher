#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 256

int *CipherCodeEncryption(unsigned char *, int);
int *CipherCodeDecryption(unsigned char *, int);
unsigned char *OpenFile(char *);
void WriteFile(int *, int, char *);
void CountChar(unsigned char *, char *);
void Menu(char *, int *);
void BruteForce();

int main()
{
    char option;
    char *filename = calloc(SIZE, sizeof(char));
    int *keySizeCipherCode = malloc(sizeof(int));

    printf("########## CAESAR CIPHER ##########\n");
    printf("###################################\n\n");
    printf("[E] - ENCRYPT TEXT\n");
    printf("[D] - DECRYPT TEXT\n");
    printf("[B] - BRUTE FORCE\n\n");

    printf("OPTION - ");
    scanf("%c", &option);

    switch (option)
    {
    case 'E':
        Menu(filename, keySizeCipherCode);

        int *cipherCodeEncryptedText = CipherCodeEncryption(OpenFile(filename), *keySizeCipherCode);
        free(keySizeCipherCode);

        WriteFile(cipherCodeEncryptedText, strlen(OpenFile(filename)), "EncryptedText.txt");
        free(cipherCodeEncryptedText);

        CountChar(OpenFile("EncryptedText.txt"), "EncryptedTextStats.csv"); // Only printable ASCII
        CountChar(OpenFile(filename), "YourTextStats.csv");                 // Only printable ASCII

        break;

    case 'D':
        Menu(filename, keySizeCipherCode);

        int *cipherCodeDecryptedText = CipherCodeDecryption(OpenFile(filename), *keySizeCipherCode);
        free(keySizeCipherCode);

        WriteFile(cipherCodeDecryptedText, strlen(OpenFile(filename)), "DecryptedText.txt");
        free(cipherCodeDecryptedText);

        CountChar(OpenFile("DecryptedText.txt"), "DecryptedTextStats.csv"); // Only printable ASCII
        CountChar(OpenFile(filename), "YourTextStats.csv");                 // Only printable ASCII

        break;

    case 'B':
        BruteForce();
        break;

    default:
        printf("WRONG OPTION");
        printf("\n\n");
        system("pause");
        exit(0);
        break;
    }

    free(filename);

    printf("DONE\n\n");

    system("pause");
    return 0;
}

int *CipherCodeEncryption(unsigned char *encryptedText, int keySizeCipherCode)
{
    int *encryptedTextArray = (int *)calloc(strlen(encryptedText), sizeof(int));

    for (int i = 0; encryptedText[i] != '\0'; i++)
        encryptedTextArray[i] = (encryptedText[i] + (keySizeCipherCode % 256)) % 256;

    return encryptedTextArray;
}

int *CipherCodeDecryption(unsigned char *decryptedText, int keySizeCipherCode)
{
    int *decryptedTextArray = (int *)calloc(strlen(decryptedText), sizeof(int));

    for (int i = 0; decryptedText[i] != '\0'; i++)
        decryptedTextArray[i] = (decryptedText[i] - (keySizeCipherCode % 256) + 256) % 256;

    return decryptedTextArray;
}

unsigned char *OpenFile(char *filename)
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        printf("FILE DOES NOT EXIST\n\n");
        system("pause");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    int lenght = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *text = (unsigned char *)calloc(lenght + 1, sizeof(unsigned char));
    int c;
    int i = 0;

    while ((c = fgetc(file)) != EOF)
    {
        text[i] = c;
        i++;
    }

    text[i] = '\0';

    fclose(file);

    return text;
}

void WriteFile(int *text, int size, char *filename)
{
    FILE *file = fopen(filename, "w+");

    for (int i = 0; i < size; i++)
        fputc(text[i], file);

    fputc(0, file);

    fclose(file);
}

void CountChar(unsigned char *Text, char *filename)
{
    int freq[256] = {0};
    FILE *file = fopen(filename, "w+");

    for (int i = 0; Text[i] != '\0'; i++)
        freq[Text[i]]++;

    for (int i = 0; i < 256; i++)
    {
        if (freq[i] != 0)
            fprintf(file, "%c;%d\n", i, freq[i]);
    }

    fclose(file);
}

void Menu(char *filename, int *keySizeCipherCode)
{
    printf("\n########## CAESAR CIPHER ##########\n");
    printf("###################################\n\n");
    printf("ENTER THE FILE NAME - ");
    scanf("%s", filename);
    printf("ENTER THE KEY SIZE - ");
    scanf("%d", keySizeCipherCode);
    printf("\n");
}

void BruteForce()
{
    char filename[SIZE];

    printf("\n########## CAESAR CIPHER ##########\n");
    printf("###################################\n\n");
    printf("ENTER THE FILE NAME - ");
    scanf("%s", filename);
    printf("\n");

    unsigned char *bruteForceText = OpenFile(filename);
    FILE *file = fopen("BruteForceDecryptedText.txt", "w+");

    for (int i = 0; i < 256; i++)
    {
        int *bruteForceDecryptedText = CipherCodeDecryption(bruteForceText, i);

        for (int j = 0; j < strlen(bruteForceText); j++)
            fputc(bruteForceDecryptedText[j], file);

        fputs("\n\n", file);

        free(bruteForceDecryptedText);
    }

    fclose(file);
    free(bruteForceText);
}