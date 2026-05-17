/*
 * ============================================================
 *   BANK ACCOUNT MANAGEMENT SYSTEM
 *   Features: Deposit | Withdraw | Balance Enquiry | Exit
 *   Concepts: Structures | Functions | File Handling
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ─────────────────────────────────────────────
   STRUCTURE DEFINITION
   ───────────────────────────────────────────── */
typedef struct {
    int    account_no;
    char   name[50];
    char   account_type[20];   /* Savings / Current */
    float  balance;
} Account;

/* ─────────────────────────────────────────────
   FUNCTION PROTOTYPES
   ───────────────────────────────────────────── */
void  createAccount();
void  deposit();
void  withdraw();
void  balanceEnquiry();
void  displayAllAccounts();
int   accountExists(int acc_no);
void  printDivider();
void  printHeader();

/* ─────────────────────────────────────────────
   FILE NAME (acts as our database)
   ───────────────────────────────────────────── */
#define FILENAME "accounts.dat"

/* ==============================================================
   MAIN FUNCTION
   ============================================================== */
int main() {
    int choice;

    printHeader();

    while (1) {
        printf("\n");
        printDivider();
        printf("          MAIN MENU\n");
        printDivider();
        printf("  [1]  Create New Account\n");
        printf("  [2]  Deposit Money\n");
        printf("  [3]  Withdraw Money\n");
        printf("  [4]  Balance Enquiry\n");
        printf("  [5]  Display All Accounts\n");
        printf("  [6]  Exit\n");
        printDivider();
        printf("  Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount();      break;
            case 2: deposit();            break;
            case 3: withdraw();           break;
            case 4: balanceEnquiry();     break;
            case 5: displayAllAccounts(); break;
            case 6:
                printf("\n  Thank you for banking with us!\n");
                printDivider();
                exit(0);
            default:
                printf("\n  [!] Invalid choice. Please enter 1-6.\n");
        }
    }

    return 0;
}

/* ==============================================================
   HELPER: Print a divider line
   ============================================================== */
void printDivider() {
    printf("  ----------------------------------------\n");
}

/* ==============================================================
   HELPER: Print the banner header
   ============================================================== */
void printHeader() {
    printf("\n");
    printf("  ========================================\n");
    printf("       BANK ACCOUNT MANAGEMENT SYSTEM     \n");
    printf("  ========================================\n");
}

/* ==============================================================
   HELPER: Check if an account number already exists
   Returns 1 if found, 0 if not
   ============================================================== */
int accountExists(int acc_no) {
    FILE *fp;
    Account acc;

    fp = fopen(FILENAME, "rb");
    if (fp == NULL) return 0;

    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.account_no == acc_no) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/* ==============================================================
   FUNCTION 1: Create a New Account
   ============================================================== */
void createAccount() {
    FILE    *fp;
    Account  acc;

    printDivider();
    printf("          CREATE NEW ACCOUNT\n");
    printDivider();

    printf("  Enter Account Number  : ");
    scanf("%d", &acc.account_no);

    if (accountExists(acc.account_no)) {
        printf("\n  [!] Account No. %d already exists!\n", acc.account_no);
        return;
    }

    printf("  Enter Account Holder  : ");
    scanf(" %[^\n]", acc.name);

    printf("  Account Type\n");
    printf("    (1) Savings   (2) Current : ");
    int type_choice;
    scanf("%d", &type_choice);

    if (type_choice == 1)
        strcpy(acc.account_type, "Savings");
    else if (type_choice == 2)
        strcpy(acc.account_type, "Current");
    else {
        printf("\n  [!] Invalid type. Defaulting to Savings.\n");
        strcpy(acc.account_type, "Savings");
    }

    printf("  Opening Balance (Rs.)  : ");
    scanf("%f", &acc.balance);

    if (acc.balance < 0) {
        printf("\n  [!] Balance cannot be negative.\n");
        return;
    }

    fp = fopen(FILENAME, "ab");
    if (fp == NULL) {
        printf("\n  [!] Error: Could not open file.\n");
        return;
    }

    fwrite(&acc, sizeof(Account), 1, fp);
    fclose(fp);

    printf("\n  [OK] Account No. %d created successfully!\n", acc.account_no);
    printDivider();
}

/* ==============================================================
   FUNCTION 2: Deposit Money
   ============================================================== */
void deposit() {
    FILE    *fp;
    Account  acc;
    int      acc_no, found = 0;
    float    amount;

    printDivider();
    printf("          DEPOSIT MONEY\n");
    printDivider();
    printf("  Enter Account Number : ");
    scanf("%d", &acc_no);

    fp = fopen(FILENAME, "r+b");
    if (fp == NULL) {
        printf("\n  [!] No records found. Create an account first.\n");
        return;
    }

    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.account_no == acc_no) {
            found = 1;

            printf("  Account Holder       : %s\n", acc.name);
            printf("  Current Balance (Rs.): %.2f\n", acc.balance);
            printf("  Deposit Amount (Rs.) : ");
            scanf("%f", &amount);

            if (amount <= 0) {
                printf("\n  [!] Deposit amount must be positive.\n");
                fclose(fp);
                return;
            }

            acc.balance += amount;

            /* Move file pointer back to overwrite this record */
            fseek(fp, -(long)sizeof(Account), SEEK_CUR);
            fwrite(&acc, sizeof(Account), 1, fp);

            printf("\n  [OK] Rs. %.2f deposited successfully!\n", amount);
            printf("  New Balance (Rs.)    : %.2f\n", acc.balance);
            printDivider();
            break;
        }
    }

    fclose(fp);

    if (!found)
        printf("\n  [!] Account No. %d not found.\n", acc_no);
}

/* ==============================================================
   FUNCTION 3: Withdraw Money
   ============================================================== */
void withdraw() {
    FILE    *fp;
    Account  acc;
    int      acc_no, found = 0;
    float    amount;

    printDivider();
    printf("          WITHDRAW MONEY\n");
    printDivider();
    printf("  Enter Account Number : ");
    scanf("%d", &acc_no);

    fp = fopen(FILENAME, "r+b");
    if (fp == NULL) {
        printf("\n  [!] No records found. Create an account first.\n");
        return;
    }

    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.account_no == acc_no) {
            found = 1;

            printf("  Account Holder       : %s\n", acc.name);
            printf("  Current Balance (Rs.): %.2f\n", acc.balance);
            printf("  Withdraw Amount (Rs.): ");
            scanf("%f", &amount);

            if (amount <= 0) {
                printf("\n  [!] Withdrawal amount must be positive.\n");
                fclose(fp);
                return;
            }

            if (amount > acc.balance) {
                printf("\n  [!] Insufficient balance!\n");
                printf("  Available Balance    : Rs. %.2f\n", acc.balance);
                fclose(fp);
                return;
            }

            if ((acc.balance - amount) < 500) {
                printf("\n  [!] Cannot withdraw. Minimum balance of Rs. 500 required.\n");
                printf("  Max withdrawable     : Rs. %.2f\n", acc.balance - 500);
                fclose(fp);
                return;
            }

            acc.balance -= amount;

            fseek(fp, -(long)sizeof(Account), SEEK_CUR);
            fwrite(&acc, sizeof(Account), 1, fp);

            printf("\n  [OK] Rs. %.2f withdrawn successfully!\n", amount);
            printf("  Remaining Balance    : Rs. %.2f\n", acc.balance);
            printDivider();
            break;
        }
    }

    fclose(fp);

    if (!found)
        printf("\n  [!] Account No. %d not found.\n", acc_no);
}

/* ==============================================================
   FUNCTION 4: Balance Enquiry
   ============================================================== */
void balanceEnquiry() {
    FILE    *fp;
    Account  acc;
    int      acc_no, found = 0;

    printDivider();
    printf("          BALANCE ENQUIRY\n");
    printDivider();
    printf("  Enter Account Number : ");
    scanf("%d", &acc_no);

    fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("\n  [!] No records found. Create an account first.\n");
        return;
    }

    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.account_no == acc_no) {
            found = 1;
            printf("\n");
            printf("  +----------------------------------+\n");
            printf("  |        ACCOUNT DETAILS           |\n");
            printf("  +----------------------------------+\n");
            printf("  |  Account No.  : %-16d  |\n", acc.account_no);
            printf("  |  Name         : %-16s  |\n", acc.name);
            printf("  |  Type         : %-16s  |\n", acc.account_type);
            printf("  |  Balance (Rs.): %-16.2f  |\n", acc.balance);
            printf("  +----------------------------------+\n");
            break;
        }
    }

    fclose(fp);

    if (!found)
        printf("\n  [!] Account No. %d not found.\n", acc_no);
}

/* ==============================================================
   FUNCTION 5 (Bonus): Display All Accounts
   ============================================================== */
void displayAllAccounts() {
    FILE    *fp;
    Account  acc;
    int      count = 0;

    printDivider();
    printf("          ALL ACCOUNTS\n");
    printDivider();

    fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("\n  [!] No records found. Create an account first.\n");
        return;
    }

    printf("  %-6s  %-20s  %-10s  %-12s\n",
           "Acc No", "Name", "Type", "Balance(Rs.)");
    printDivider();

    while (fread(&acc, sizeof(Account), 1, fp)) {
        printf("  %-6d  %-20s  %-10s  %-12.2f\n",
               acc.account_no,
               acc.name,
               acc.account_type,
               acc.balance);
        count++;
    }

    fclose(fp);
    printDivider();

    if (count == 0)
        printf("  No accounts to display.\n");
    else
        printf("  Total accounts: %d\n", count);

    printDivider();
}

/* ─────────────────────────────────────────────
   END OF PROGRAM
   ───────────────────────────────────────────── */