/*
 * Module Name:server.c
 * Description: source file of the server module
 *  Created on: Sep 15, 2022
 *      Author: Mahmoud Yasser
 */
/*******************************************************************************
 *                                 Includes                                    *
 *******************************************************************************/
#include "server.h"
#include <stdio.h>
#include <string.h>
/*******************************************************************************
 *                             Static Prototypes                               *
 *******************************************************************************/
/*
 * Function Name: copyTransaction
 * Description: a static function to copy all data of source transaction into a destination transaction
 */
static void copyTransaction(ST_transaction_t *destinationTrans,
		ST_transaction_t *sourceTrans);

/*******************************************************************************
 *                            Static Global Variables                          *
 *******************************************************************************/
/* database of valid accounts */
static ST_accountsDB_t validAccountsDB[ACCOUNTS_DB_SIZE] = { { 4000.0, RUNNING,
		"4847352989263094" }, { 15000.0, RUNNING, "3379513561108795" }, {
		9000.0, BLOCKED, "5181975718047403" }, { 2000.0, RUNNING,
		"8989374615436851" }, { 100000.0, BLOCKED, "5807007076043875" } };

/* database of transactions */
static ST_transaction_t transactionsDB[TRANSACTIONS_DB_SIZE];
/* index of last free position in the transaction database */
static uint8_t transactionDB_last_free_index = 0;
/* the sequence number of the transaction */
static uint32_t transaction_sequence_num = 0;

/*******************************************************************************
 *                                Functions                                    *
 *******************************************************************************/

EN_serverError_t isValidAccount(ST_cardData_t *cardData) {
	EN_serverError_t status = ACCOUNT_NOT_FOUND;
	if (cardData != NULL) {
		for (uint8_t i = 0;
				((i < ACCOUNTS_DB_SIZE) && (status == ACCOUNT_NOT_FOUND));
				i++) {
			/* if the PAN does not exist in the valid accounts database, the account is invalid */
			if (strcmp(cardData->primaryAccountNumber,
					validAccountsDB[i].primaryAccountNumber) == 0) {
				status = SERVER_OK;
			}
		}
	}
	return status;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence) {
	EN_serverError_t status = BLOCKED_ACCOUNT;
	/* looping in the valid accounts database until the account PAN is found */
	for (uint8_t i = 0; ((i < ACCOUNTS_DB_SIZE) && (status == BLOCKED_ACCOUNT));
			i++) {
		if (strcmp(accountRefrence->primaryAccountNumber,
				validAccountsDB[i].primaryAccountNumber) == 0) {
			/* if the account with same PAN is found, check if its state is RUNNING */
			if (validAccountsDB[i].state == RUNNING) {
				/* if the state of the account is running, the status is SERVER_OK */
				status = SERVER_OK;
			}
		}
	}
	return status;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData,
		ST_cardData_t *cardData) {
	uint8_t status = LOW_BALANCE;
	if ((termData != NULL) && (cardData != NULL)) {
		/*
		 * looping through the valid accounts database until the PAN of the user making
		 * the transaction is reached
		 */
		for (uint8_t i = 0; i < ACCOUNTS_DB_SIZE; i++) {
			if (strcmp(cardData->primaryAccountNumber,
					validAccountsDB[i].primaryAccountNumber) == 0) {
				/* checking if the transaction amount is less than or equal to user's balance */
				if (termData->transAmount <= validAccountsDB[i].balance) {
					status = SERVER_OK;
				}
			}
		}
	}
	return status;
}

static void copyTransaction(ST_transaction_t *destinationTrans,
		ST_transaction_t *sourceTrans) {
	/* copying card holder data from source to destination */
	strcpy(destinationTrans->cardHolderData.cardExpirationDate,
			sourceTrans->cardHolderData.cardExpirationDate);
	strcpy(destinationTrans->cardHolderData.cardHolderName,
			sourceTrans->cardHolderData.cardHolderName);
	strcpy(destinationTrans->cardHolderData.primaryAccountNumber,
			sourceTrans->cardHolderData.primaryAccountNumber);
	/* copying terminal data from source to destination */
	strcpy(destinationTrans->terminalData.transactionDate,
			sourceTrans->terminalData.transactionDate);
	destinationTrans->terminalData.maxTransAmount =
			sourceTrans->terminalData.maxTransAmount;
	destinationTrans->terminalData.transAmount =
			sourceTrans->terminalData.transAmount;
	/* copying transaction sequence number from source to destination */
	destinationTrans->transactionSequenceNumber =
			sourceTrans->transactionSequenceNumber;

	destinationTrans->transState = sourceTrans->transState;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData) {
	EN_serverError_t status = SERVER_OK;
	EN_serverError_t getTransactionStatus = SERVER_OK;
	ST_transaction_t savedTransaction;
	/* check if the database is not full */
	if ((transData != NULL)
			&& (transactionDB_last_free_index <= TRANSACTIONS_DB_SIZE - 1)) {
		/* giving the transaction a sequence number */
		transData->transactionSequenceNumber = transaction_sequence_num;
		/* copying the transaction into the transaction database */
		copyTransaction(&transactionsDB[transactionDB_last_free_index],
				transData);
		/* incrementing the global sequence number and the last free index */
		transaction_sequence_num++;
		transactionDB_last_free_index++;
		/* check if the transaction is saved */
		getTransactionStatus = getTransaction(
				transData->transactionSequenceNumber, &savedTransaction);
		if (getTransactionStatus != SERVER_OK) {
			status = SAVING_FAILED;
		}

	} else {
		status = SAVING_FAILED;
	}
	return status;
}

EN_transState_t receiveTransactionData(ST_transaction_t *transData) {
	EN_transState_t status = APPROVED;
	EN_serverError_t serverStatus = SERVER_OK;
	ST_accountsDB_t account;
	if (transData != NULL) {
		/* looping until the PAN is found in the accounts database */
		for (uint8_t i = 0; i < ACCOUNTS_DB_SIZE; i++) {
			if (strcmp(transData->cardHolderData.primaryAccountNumber,
					validAccountsDB[i].primaryAccountNumber) == 0) {
				/* if the account is found, its data is copied to account object */
				account.balance = validAccountsDB[i].balance;
				account.state = validAccountsDB[i].state;
				strcpy(account.primaryAccountNumber,
						validAccountsDB[i].primaryAccountNumber);
			}
		}

		/* checking if the card is invalid */
		if (isValidAccount(&(transData->cardHolderData)) == ACCOUNT_NOT_FOUND) {
			status = FRAUD_CARD;
		}
		/* checking the amount is valid */
		else if (isAmountAvailable((&(transData->terminalData)),
				(&(transData->cardHolderData))) == LOW_BALANCE) {
			status = DECLINED_INSUFFECIENT_FUND;
		} else if (isBlockedAccount(&account) == BLOCKED_ACCOUNT) {
			status = DECLINED_STOLEN_CARD;
		}
		/* checking if the transaction saving failed */
		transData->transState = status;
		serverStatus = saveTransaction(transData);
		if (serverStatus == SAVING_FAILED) {
			printf("%s\n", "Server Down");
			return INTERNAL_SERVER_ERROR;
		} else {
			/* looping through the valid accounts database until the PAN of the user making
			 * the transaction is reached
			 * then subtracting the transaction amount from user's balance */
			if (status == APPROVED) {
				for (uint8_t i = 0; i < ACCOUNTS_DB_SIZE; i++) {
					if (strcmp(transData->cardHolderData.primaryAccountNumber,
							validAccountsDB[i].primaryAccountNumber) == 0) {
						printf("%s%.2f\n", "Your balance was: ",
								validAccountsDB[i].balance);
						validAccountsDB[i].balance -=
								transData->terminalData.transAmount;
						printf("%s%.2f\n", "Now your balance is: ",
								validAccountsDB[i].balance);
					}
				}
			}
		}
	}
	return status;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber,
		ST_transaction_t *transData) {
	EN_serverError_t status = TRANSACTION_NOT_FOUND;
	/* looping in the transaction database until the transaction with
	 * the given sequence number is found
	 */
	for (uint8_t i = 0; i < transactionDB_last_free_index; i++) {
		/* if found, the transaction is copied into the database*/
		if (transactionsDB[i].transactionSequenceNumber
				== transactionSequenceNumber) {
			copyTransaction(transData, &transactionsDB[i]);
			status = SERVER_OK;
		}
	}
	return status;
}

void printTransaction(ST_transaction_t *transData) {
	printf("%s\n", "Printing a Transaction: ");
	printf("%s%s\n", "Card holder name: ",
			transData->cardHolderData.cardHolderName);
	printf("%s%s\n", "Card expiration date: ",
			transData->cardHolderData.cardExpirationDate);
	printf("%s%s\n", "Card PAN: ",
			transData->cardHolderData.primaryAccountNumber);
	printf("%s%.2f\n", "Maximum transaction amount: ",
			transData->terminalData.maxTransAmount);
	printf("%s%.2f\n", "Transaction amount: ",
			transData->terminalData.transAmount);
	printf("%s%s\n", "Transaction date: ",
			transData->terminalData.transactionDate);
	switch (transData->transState) {
	case APPROVED:
		printf("%s%s\n", "Transaction State: ", "Approved");
		break;

	case DECLINED_STOLEN_CARD:
		printf("%s%s\n", "Transaction State: ", "Declined stolen card");
		break;

	case DECLINED_INSUFFECIENT_FUND:
		printf("%s%s\n", "Transaction State: ", "Insufficient fund");
		break;
	case FRAUD_CARD:
		printf("%s%s\n", "Transaction State: ", "Fraud Card");
		break;
	}
	printf("%s%d\n", "Transaction Sequence Number: ",
			transData->transactionSequenceNumber);
}
