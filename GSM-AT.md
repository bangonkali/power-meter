# AT COMMANDS DIRECTORY

## Get phone number.
```
AT+CPBS="ON"
AT+CPBR=1
```

## Read a message

Read unread message. 

```
AT+CMGL="REC UNREAD"
```

Read all message

```
AT+CMGL="ALL"
```

Read specific message

```
AT+CMGR=3
```

### Source

http://www.developershome.com/sms/readSmsByAtCommands.asp

## Deleting Message

### Source

http://www.developershome.com/sms/cmgdCommand.asp

## Send message in TEXT Mode
```
AT+CMGF=1
OK
AT+CMGS="+639157764387"
> This is the text message.
+CMGS: 198
OK
```

## AT+CMGF – Set SMS Text Mode or SMS PDU Mode

The AT+CMGF command sets the GSM modem in SMS Text Mode or SMS PDU Mode.

In Text Mode, SMS messages are represented as readable text. In PDU Mode, all SMS messages are represented as binary strings encoded in hexadecimal characters like 31020B911326880736F40000A900.

Although Text Mode is easier to use, PDU Mode is more consistent on different GSM Modems.

| Command | Positive Response |
| AT+CMGF=<mode><CR> | OK |

Parameters
```
<mode>: 0 = PDU Mode, 1 = Text Mode
<CR> = ASCII character 13
```

### Example

```
AT+CMGF=0	 set SMS PDU mode on
OK
AT+CNMI=1,2	 forward received SMS-DELIVERs to TE
OK
AT+CMGS=29	 length of the SMS PDU
079153588077010111000C915358508282420000A71154747A0E4ACf41f4f29C9E769F4121
+CMGS: 212	 message reference is shown
OK
+CMT: ,34
0791535880770101200C915358508282420000691170413375081154747A0E4ACF41F4F29C9E769F4121
```

### Explanation of the PDU:

07 91	address length: 7, type of address is international using ISDN telephone numbering plan
53 58 80 77 01 01	SC address: 358508771010
01	SMS-SUBMIT, do not reject duplicates in SC, validity period not present, no status report request, no header in user data, reply path not set
00	TP message reference, given by the phone
0A A1	address length: 10, type of address is national using ISDN telephone numbering plan
50 50 82 82 42	destination address: 0505282824
00 00	protocol identifier: straightforward, data coding scheme: default class, 7 bit message
11 54 74 7A 0E 4A CF 41 F4 F2 9C 9E 76 9F 41 21	length of user data 17, user data

8421 8421
0101 0101 octet, 0*8+1*4+0*2+1*1=5
5 5 hex
16*5+5=85 decimal

### Source

http://developer.nokia.com/Community/Discussion/showthread.php/4377-Send-in-PDU-Mode-using-AT-commands