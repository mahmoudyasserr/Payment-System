/*
 * Module Name: server.h
 * Description: header file of the server module
 *  Created on: Sep 15, 2022
 *      Author: Mahmoud Yasser
 */

#ifndef SERVER_H_
#define SERVER_H_

/*******************************************************************************
 *                                  Includes                                   *
 *******************************************************************************/
#include <stdint.h>
#include "../Card/card.h"
#include "../Terminal/terminal.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define 		ACCOUNTS_DB_SIZE				(255)
#define 		TRANSACTIONS_DB_SIZE			(255)
/*******************************************************************************
 *                              Defined Types                                  *
 *******************************************************************************/

typedef enum EN_transState_t {
	APPROVED,
	DECLINED_INSUFFECIENT_FUND,
	DECLINED_STOLEN_CARD,
	FRAUD_CARD,
	INTERNAL_SERVER_ERROR
} EN_transState_t;

typedef struct ST_transaction_t {
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transState_t transState;
	uint32_t transactionSequenceNumber;
} ST_transaction_t;

typedef enum EN_serverError_t {
	SERVER_OK,
	SAVING_FAILED,
	TRANSACTION_NOT_FOUND,
	ACCOUNT_NOT_FOUND,
	LOW_BALANCE,
	BLOCKED_ACCOUNT
} EN_serverError_t;

typedef enum EN_accountState_t {
	RUNNING, BLOCKED
} EN_accountState_t;

typedef struct ST_accountsDB_t {
	float balance;
	EN_accountState_t state;
	uint8_t primaryAccountNumber[20];
} ST_accountsDB_t;

/*******************************************************************************
 *                      	Functions Prototypes                               *
 *******************************************************************************/
/*
 * Function Name: isValidAccount
 * Description: This function will take card data and validate these data.
 * It checks if the PAN exists or not in the server's database.
 * If the PAN doesn't exist will return ACCOUNT_NOT_FOUND, else will return SERVER_OK
 */
EN_serverError_t isValidAccount(ST_cardData_t *cardData);

/*
 * Function Name: isBlockedAccount
 * Description: This function will check whether an account is blocked or not
 * If the account is blocked, it will return BLOCKED_ACCOUNT
 * else it will return SERVER_OK
 */
EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence);

/*
 * Function Name: isAmountAvailable
 * Description: This function will take terminal data and validate these data.
 * It checks if the transaction's amount is available or not.
 * If the transaction amount is greater than the balance in the database will return LOW_BALANCE,
 * else will return SERVER_OK
 */
EN_serverError_t isAmountAvailable(ST_terminalData_t *termData,
		ST_cardData_t *cardData);

/*
 * Function Name: saveTransaction
 * Description: This function will store all transaction data in the transactions database.
 * It gives a sequence number to a transaction,
 * this number is incremented once a transaction is processed into the server,
 * you must check the last sequence number in the server
 * to give the new transaction a new sequence number.
 * It saves any type of transactions, APPROVED or DECLINED,
 * with the specific reason for declining/transaction state.
 * If the transaction can't be saved, will return SAVING_FAILED,
 * else will return SERVER_OK,
 * It checks if the transaction is saved or not using the getTransaction function.
 */

EN_serverError_t saveTransaction(ST_transaction_t *transData);

/*
 * This function will take all transaction data and validate its data.
 * It checks the account details and amount availability.
 * If the account does not exist return FRAUD_CARD,
 * if the amount is not available will return DECLINED_INSUFFECIENT_FUND,
 * if the account is blocked will return DECLINED_STOLEN_CARD,
 * if a transaction can't be saved will return INTERNAL_SERVER_ERROR and will not save the transaction,
 * else returns APPROVED.
 * It will update the database with the new balance.
*/
EN_transState_t receiveTransactionData(ST_transaction_t *transData);

/*
 * Function Name: getTransaction
 * Description:This function will take a transaction sequence number
 * and search for this transaction in the database.
 * If the transaction can't be found will return TRANSACTION_NOT_FOUND,
 * else will return SERVER_OK and store the transaction in a structure
 */
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber,
		ST_transaction_t *transData);

/*
 * Function Name: printTransaction
 * Description: This function will print a transaction
 */
void printTransaction(ST_transaction_t *transData);
#endif /* SERVER_H_ */
