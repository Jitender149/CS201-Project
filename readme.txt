--------CS201 Read Me-----------
Code is written in simple cpp language and compiled with standard gnu compiler
Our code simulates a blockchain for a fixed 4 users, using standard SHA512 function for encryption
Our input is in form of a menu-which gives you the option to 
1)Add a transaction to the blockchain
2)Display one of the local copies
3)Simulate an attack on the blockchain (i.e. be a hacker)

First of all you are asked to enter the 4 usernames which will be the name of the users in the p2p networks
Then you will be given 3 options.
When you select 1.Add a transaction then you have to enter the name of the users between whom transaction is taking place and the amount.
then it will run a consensus in which you have to tell Y\N i.e. if you validate the transaction or not. This will ber asked from all the four user, including the one who is adding the transaction.
Then if you get the majority then block gets added in the blockchain.
To again add a block repeat the same.

when you select 2. This will ask to enter the username whose local copy you want to display and then would display it accordingly.

when you select 3. This will ask the name of the user whose local copy you want to change then it displays the number of block(for hacker's reference so that you don't enter wayward block no in the next line). Then it asks the block no you want to make changes in. Once you enter all this this will ask you the complete detail about the transaction as done above in case 1. Then it will simply check using the algo if this is a valid change or not and show you the display accordingly.
To get a better behold of this you can print the blockchain before and after hacker's action.
 

