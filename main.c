/*
 * Module Name: main.c
 * Description: main function of the application
 *  Created on: Sep 18, 2022
 *      Author: Mahmoud Yasser
 */
/*******************************************************************************
 *                                 Includes                                    *
 *******************************************************************************/
#include "main.h"
#include "Card/card.h"
#include "Terminal/terminal.h"
#include "Server/server.h"
#include "Application/app.h"
#include "Eclipse_Config.h"
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
/*******************************************************************************
 *                              Static Prototypes                              *
 *******************************************************************************/
#if	(CURRENTLY_RUNNING == GET_CARD_HOLDER_NAME_TESTING)
static void getCardNameTesting(void);
#endif
#if	(CURRENTLY_RUNNING == GET_CARD_EXPIRY_DATE_TESTING)
static void getCardExpiryDateTesting(void);
#endif
#if	(CURRENTLY_RUNNING == GET_CARD_PAN_TESTING)
static void getCardPANTesting(void);
#endif
#if	(CURRENTLY_RUNNING == GET_TRANSACTION_DATE_TESTING)
static void getTransactionDateTesting(void);
#endif
#if	(CURRENTLY_RUNNING == IS_CARD_EXPIRED_TESTING)
static void isCardExpiredTesting(void);
#endif
#if	(CURRENTLY_RUNNING == IS_VALID_CARD_PAN_TESTING)
static void isValidCardPanTesting(void);
#endif
#if	(CURRENTLY_RUNNING == GET_TRANSACTION_AMOUNT_TESTING)
static void getTransactionAmountTesting(void);
#endif
#if	(CURRENTLY_RUNNING == IS_BELOW_MAX_AMOUNT_TESTING)
static void isBelowMaxAmountTesting(void);
#endif
#if	(CURRENTLY_RUNNING == SET_MAX_AMOUNT_TESTING)
static void setMaxAmountTesting(void);
#endif
#if	(CURRENTLY_RUNNING == RECEIVE_TRANSACTION_DATA_TESTING)
static void receiveTransactionDataTesting(void);
#endif
#if	(CURRENTLY_RUNNING == IS_VALID_ACCOUNT_TESTING)
static void isValidAccountTesting(void);
#endif
#if	(CURRENTLY_RUNNING == IS_AMOUNT_AVAILABE_TESTING)
static void isAmountAvailabeTesting(void);
#endif
#if	(CURRENTLY_RUNNING == SAVE_TRANSACTION_TESTING)
static void saveTransactionTesting(void);
#endif
#if	(CURRENTLY_RUNNING == GET_TRANSACTION_TESTING)
static void getTransactionTesting(void);
#endif

/*******************************************************************************
 *                                Main Function                                *
 *******************************************************************************/
int main(void) {
#if	(CURRENTLY_RUNNING == GET_CARD_HOLDER_NAME_TESTING)
	getCardNameTesting();
#elif (CURRENTLY_RUNNING == GET_CARD_EXPIRY_DATE_TESTING)
	getCardExpiryDateTesting();
#elif (CURRENTLY_RUNNING == GET_CARD_PAN_TESTING)
	getCardPANTesting();
#elif (CURRENTLY_RUNNING == GET_TRANSACTION_DATE_TESTING)
	getTransactionDateTesting();
#elif (CURRENTLY_RUNNING == IS_CARD_EXPIRED_TESTING)
	isCardExpiredTesting();
#elif (CURRENTLY_RUNNING == IS_VALID_CARD_PAN_TESTING)
	isValidCardPanTesting();
#elif (CURRENTLY_RUNNING == GET_TRANSACTION_AMOUNT_TESTING)
	getTransactionAmountTesting();
#elif (CURRENTLY_RUNNING == IS_BELOW_MAX_AMOUNT_TESTING)
	isBelowMaxAmountTesting();
#elif (CURRENTLY_RUNNING == SET_MAX_AMOUNT_TESTING)
	setMaxAmountTesting();
#elif (CURRENTLY_RUNNING == RECEIVE_TRANSACTION_DATA_TESTING)
	receiveTransactionDataTesting();
#elif (CURRENTLY_RUNNING == IS_VALID_ACCOUNT_TESTING)
	isValidAccountTesting();
#elif (CURRENTLY_RUNNING == IS_AMOUNT_AVAILABE_TESTING)
	isAmountAvailabeTesting();
#elif (CURRENTLY_RUNNING == SAVE_TRANSACTION_TESTING)
	saveTransactionTesting();
#elif (CURRENTLY_RUNNING == GET_TRANSACTION_TESTING)
	getTransactionTesting();
#elif (CURRENTLY_RUNNING == APPLICATION)
	uint8_t option = 0;
		while ((option != 'b') && (option != 'B')) {
			printf("%s\n", "Choose Option:");
			printf("%s\n", "a: Insert Card");
			printf("%s\n", "b: Power OFF");
	#if (ECLIPSE_USED == 1)
			fflush(stdout);
	#endif
			scanf(" %c", &option);
			if ((option == 'a') || (option == 'A')) {
				appStart();
			} else if ((option == 'b') || (option == 'B')) {
				printf("%s\n", "Terminal is shutting down...");
			} else {
				printf("%s\n", "Wrong Option");
			}
		}
#endif

	return 0;
}

/*******************************************************************************
 *                               Function Testing                              *
 *******************************************************************************/
#if (CURRENTLY_RUNNING == GET_CARD_HOLDER_NAME_TESTING)
static void getCardNameTesting(void) {
	EN_cardError_t status = CARD_OK;
	ST_cardData_t cardData;
	printf("%s\n", "Press Enter before entering the name");
	status = getCardHolderName(&cardData);
	if (status == WRONG_NAME) {
		printf("%s\n", "Card Name is WRONG");
	} else {
		printf("%s\n", "Card Name is OK");
	}
}
#endif

#if (CURRENTLY_RUNNING == GET_CARD_EXPIRY_DATE_TESTING)
static void getCardExpiryDateTesting(void) {
	EN_cardError_t status = CARD_OK;
	ST_cardData_t cardData;
	status = getCardExpiryDate(&cardData);
	if (status == WRONG_EXP_DATE) {
		printf("%s\n", "Card Date is WRONG");
	} else {
		printf("%s\n", "Card Date is OK");
	}
}
#endif

#if (CURRENTLY_RUNNING == GET_CARD_PAN_TESTING)
static void getCardPANTesting(void) {
	EN_cardError_t status = CARD_OK;
	ST_cardData_t cardData;
	status = getCardPAN(&cardData);
	if (status == WRONG_PAN) {
		printf("%s\n", "Card PAN is WRONG");
	} else {
		printf("%s\n", "Card PAN is OK");
	}
}
#endif

#if (CURRENTLY_RUNNING == GET_TRANSACTION_DATE_TESTING)
static void getTransactionDateTesting(void) {
	ST_terminalData_t termData;
	EN_terminalError_t status = TERMINAL_OK;
	getTransactionDate(&termData);
	printf("%s%s", "Transaction Date is: ", termData.transactionDate);
}
#endif

#if (CURRENTLY_RUNNING == IS_CARD_EXPIRED_TESTING)
static void isCardExpiredTesting(void) {
	EN_cardError_t expStatus = CARD_OK;
	EN_terminalError_t status = TERMINAL_OK;
	ST_terminalData_t termData;
	ST_cardData_t cardData;
	getTransactionDate(&termData);
	expStatus = getCardExpiryDate(&cardData);
	if (expStatus == WRONG_EXP_DATE) {
		printf("%s\n", "Card Date is WRONG");
		return;
	} else {
		printf("%s\n", "Card Date is OK");
	}
	status = isCardExpired(cardData, termData);
	if (status == EXPIRED_CARD) {
		printf("%s\n", "Card is Expired");
	} else {
		printf("%s\n", "Card is not Expired");
	}
}
#endif

#if (CURRENTLY_RUNNING == IS_VALID_CARD_PAN_TESTING)
static void isValidCardPanTesting(void) {
	EN_cardError_t cardStatus = CARD_OK;
	EN_terminalError_t status = TERMINAL_OK;
	ST_cardData_t cardDate;
	cardStatus = getCardPAN(&cardDate);
	if (cardStatus == WRONG_PAN) {
		printf("%s\n", "PAN is WRONG");
		return;
	} else {
		printf("%s\n", "PAN is OK");
	}
	status = isValidCardPAN(&cardDate);
	if (status == INVALID_CARD) {
		printf("%s\n", "PAN is not Luhn number");
	} else {
		printf("%s\n", "PAN is Luhn number");
	}
}
#endif

#if (CURRENTLY_RUNNING == GET_TRANSACTION_AMOUNT_TESTING)
static void getTransactionAmountTesting(void) {
	EN_terminalError_t status = TERMINAL_OK;
	ST_terminalData_t termData;
	status = getTransactionAmount(&termData);
	if (status == INVALID_AMOUNT) {
		printf("%s\n", "Invalid transaction amount");
	} else {
		printf("%s\n", "Valid transaction amount");
	}
}
#endif

#if (CURRENTLY_RUNNING == IS_BELOW_MAX_AMOUNT_TESTING)
static void isBelowMaxAmountTesting(void) {
	EN_terminalError_t status = TERMINAL_OK;
	EN_terminalError_t getTransactionStatus = TERMINAL_OK;
	ST_terminalData_t termData;
	termData.maxTransAmount = MAX_AMOUNT;
	getTransactionStatus = getTransactionAmount(&termData);
	if (getTransactionStatus == INVALID_AMOUNT) {
		printf("%s\n", "Invalid transaction amount");
		return;
	} else {
		printf("%s\n", "Valid transaction amount");
	}
	status = isBelowMaxAmount(&termData);
	if (status == EXCEED_MAX_AMOUNT) {
		printf("%s\n", "Transaction amount exceeds maximum amount");
	} else {
		printf("%s\n", "Transaction amount does not exceed maximum amount");
	}
}
#endif

#if (CURRENTLY_RUNNING == SET_MAX_AMOUNT_TESTING)
static void setMaxAmountTesting(void) {
	ST_terminalData_t termData;
	setMaxAmount(&termData);
	assert(termData.maxTransAmount == MAX_AMOUNT);
}
#endif

#if (CURRENTLY_RUNNING == IS_VALID_ACCOUNT_TESTING)
static void isValidAccountTesting(void) {
	EN_cardError_t getPANStatus = CARD_OK;
	EN_serverError_t status = SERVER_OK;
	ST_cardData_t cardData;
	getPANStatus = getCardPAN(&cardData);
	if (status == WRONG_PAN) {
		printf("%s\n", "Card PAN is WRONG");
	} else {
		printf("%s\n", "Card PAN is OK");
	}
	status = isValidAccount(&cardData);
	if (status == DECLINED_STOLEN_CARD) {
		printf("%s\n", "Invalid account");
	} else {
		printf("%s\n", "Valid account");
	}
}
#endif

#if (CURRENTLY_RUNNING == IS_AMOUNT_AVAILABE_TESTING)
static void isAmountAvailabeTesting(void) {
	EN_serverError_t status = SERVER_OK;
	EN_cardError_t getPANStatus = CARD_OK;
	EN_terminalError_t getTransactionStatus = TERMINAL_OK;
	ST_cardData_t cardData;
	ST_terminalData_t termData;
	getPANStatus = getCardPAN(&cardData);
	if (status == WRONG_PAN) {
		printf("%s\n", "Card PAN is WRONG");
		return;
	} else {
		printf("%s\n", "Card PAN is OK");
	}
	getTransactionStatus = getTransactionAmount(&termData);
	if (getTransactionStatus == INVALID_AMOUNT) {
		printf("%s\n", "Invalid transaction amount");
		return;
	} else {
		printf("%s\n", "Valid transaction amount");
	}
	status = isAmountAvailable(&termData, &cardData);
	if (status == LOW_BALANCE) {
		printf("%s\n", "Amount is unavailable");
	} else {
		printf("%s\n", "Amount is available");
	}
}
#endif

#if (CURRENTLY_RUNNING == SAVE_TRANSACTION_TESTING)
static void saveTransactionTesting(void) {
	EN_serverError_t status = SERVER_OK;
	ST_transaction_t transData;
	ST_cardData_t cardData;
	ST_terminalData_t termData;
	/* getting the card holder name */
	printf("%s\n", "Press Enter then enter the name");
	if (getCardHolderName(&cardData) == WRONG_NAME) {
		printf("%s\n", "You Entered a Wrong Name");
		printf("%s\n", "Transaction Failed");
		return;
	}
	/* getting the card expiry date */
	if (getCardExpiryDate(&cardData) == WRONG_EXP_DATE) {
		printf("%s\n", "You Entered a Wrong Expiry Date");
		printf("%s\n", "Transaction Failed");
		return;
	}
	/* getting the card PAN */
	if (getCardPAN(&cardData) == WRONG_PAN) {
		printf("%s\n", "You Entered a Wrong PAN");
		printf("%s\n", "Transaction Failed");
		return;
	}
	/* checking if the PAN is Luhn number */
	if (isValidCardPAN(&cardData) == INVALID_CARD) {
		printf("%s\n", "PAN is not a Luhn Number");
		printf("%s\n", "Transaction Failed");
		return;
	}
	/* getting the transaction data (today's date) */
	if (getTransactionDate(&termData) == WRONG_DATE) {
		printf("%s\n", "Transaction date error");
		printf("%s\n", "Transaction Failed");
		return;
	}
	/* checking if the card is expired */
	if (isCardExpired(cardData, termData) == EXPIRED_CARD) {
		printf("%s\n", "The Card is Expired");
		printf("%s\n", "Transaction Failed");
		return;
	}
	/* setting the maximum transaction amount */
	if (setMaxAmount(&termData) == INVALID_MAX_AMOUNT) {
		printf("%s\n", "The maximum amount is invalid");
		printf("%s\n", "Transaction Failed");
		return;
	}
	/* getting the transaction amount */
	if (getTransactionAmount(&termData) == INVALID_AMOUNT) {
		printf("%s\n", "Invalid transaction amount");
		printf("%s\n", "Transaction Failed");
		return;
	}
	/* checking if the transaction amount exceeds the maximum amount */
	if (isBelowMaxAmount(&termData) == EXCEED_MAX_AMOUNT) {
		printf("%s %.2f\n", "Transaction amount exceeds", MAX_AMOUNT);
		printf("%s\n", "Transaction Failed");
		return;
	}

	transData.cardHolderData = cardData;
	transData.terminalData = termData;
	status = saveTransaction(&transData);
	if (status == SAVING_FAILED) {
		printf("%s\n", "Server Down");
	}
	else{
		printf("%s\n", "Transaction Saved");
	}
}
#endif

#if (CURRENTLY_RUNNING == RECEIVE_TRANSACTION_DATA_TESTING)
static void receiveTransactionDataTesting(void) {
	appStart();
}
#endif

#if (CURRENTLY_RUNNING == GET_TRANSACTION_TESTING)
static void getTransactionTesting(void) {
	ST_transaction_t transData;
	printf("%s\n", "Press Enter then enter the name");
	appStart();
	getTransaction(0, &transData);
	printTransaction(&transData);
}
#endif

