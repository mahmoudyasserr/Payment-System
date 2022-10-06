/*
 * Module Name: app.c
 * Description: the source file of the application code
 *  Created on: Sep 15, 2022
 *      Author: Mahmoud Yasser
 */

/*******************************************************************************
 *                                 Includes                                    *
 *******************************************************************************/
#include "app.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"
#include <stdio.h>
#include <stdint.h>

/*******************************************************************************
 *                                Functions                                    *
 *******************************************************************************/
void appStart(void) {
	ST_cardData_t cardData;
	ST_terminalData_t terminalData;
	ST_transaction_t transactionData;
	EN_transState_t transactionState;
	EN_appError_t okUntilNow = APP_OK;
	/* getting the card holder name */
	if (getCardHolderName(&cardData) == WRONG_NAME) {
		printf("%s\n", "You Entered a Wrong Name");
		printf("%s\n", "Transaction Failed");
		okUntilNow = APP_ERROR;
	}
	/* getting the card expiry date */
	if (okUntilNow == APP_OK) {
		if (getCardExpiryDate(&cardData) == WRONG_EXP_DATE) {
			printf("%s\n", "You Entered a Wrong Expiry Date");
			printf("%s\n", "Transaction Failed");
			okUntilNow = APP_ERROR;
		}
	}

	/* getting the card PAN */
	if (okUntilNow == APP_OK) {
		if (getCardPAN(&cardData) == WRONG_PAN) {
			printf("%s\n", "You Entered a Wrong PAN");
			printf("%s\n", "Transaction Failed");
			okUntilNow = APP_ERROR;
		}
	}

	/* checking if the PAN is Luhn number */
	if (okUntilNow == APP_OK) {
		if (isValidCardPAN(&cardData) == INVALID_CARD) {
			printf("%s\n", "PAN is not a Luhn Number");
			printf("%s\n", "Transaction Failed");
			okUntilNow = APP_ERROR;
		}
	}

	/* getting the transaction data (today's date) */
	if (okUntilNow == APP_OK) {
		if (getTransactionDate(&terminalData) == WRONG_DATE) {
			printf("%s\n", "Transaction date error");
			printf("%s\n", "Transaction Failed");
			okUntilNow = APP_ERROR;
		}
	}

	/* checking if the card is expired */
	if (okUntilNow == APP_OK) {
		if (isCardExpired(cardData, terminalData) == EXPIRED_CARD) {
			printf("%s\n", "The Card is Expired");
			printf("%s\n", "Transaction Failed");
			okUntilNow = APP_ERROR;
		}
	}

	/* setting the maximum transaction amount */
	if (okUntilNow == APP_OK) {
		if (setMaxAmount(&terminalData) == INVALID_MAX_AMOUNT) {
			printf("%s\n", "The maximum amount is invalid");
			printf("%s\n", "Transaction Failed");
			okUntilNow = APP_ERROR;
		}
	}

	/* getting the transaction amount */
	if (okUntilNow == APP_OK) {
		if (getTransactionAmount(&terminalData) == INVALID_AMOUNT) {
			printf("%s\n", "Invalid transaction amount");
			printf("%s\n", "Transaction Failed");
			okUntilNow = APP_ERROR;
		}
	}

	/* checking if the transaction amount exceeds the maximum amount */
	if (okUntilNow == APP_OK) {
		if (isBelowMaxAmount(&terminalData) == EXCEED_MAX_AMOUNT) {
			printf("%s %.2f\n", "Transaction amount exceeds", MAX_AMOUNT);
			printf("%s\n", "Transaction Failed");
			okUntilNow = APP_ERROR;
		}
	}

	if (okUntilNow == APP_OK) {
		transactionData.cardHolderData = cardData;
		transactionData.terminalData = terminalData;
		/* receiving the transaction data */
		transactionState = receiveTransactionData(&transactionData);
		/* saving the transaction if it is approved otherwise, close the transaction */
		switch (transactionState) {
		case FRAUD_CARD:
			printf("%s\n", "This is a Fraud Card.");
			printf("%s\n", "Transaction Failed");
			break;
		case DECLINED_STOLEN_CARD:
			printf("%s\n", "This account is blocked.");
			printf("%s\n", "Transaction Failed");
			break;
		case DECLINED_INSUFFECIENT_FUND:
			printf("%s\n", "Transaction amount is unavailable");
			printf("%s\n", "Transaction Failed");
			break;
		case INTERNAL_SERVER_ERROR:
			printf("%s\n", "Transaction can't be received");
			printf("%s\n", "Transaction Failed");
			break;
		case APPROVED:
			printf("%s\n", "Transaction is done successfully");
			break;
		}
	}

}
