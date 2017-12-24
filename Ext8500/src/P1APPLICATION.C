
	if (GetFromQue(P1FIFOIn, &Item))
	{
		switch(Item) {
		case P1_SELF_RLED:		case P1_SELF_GLED:		case P1_SELF_CLED:
		case P1_SELF_GPLED:		case P1_SELF_FRLED:		case P1_SELF_FGLED:
		case P1_SELF_FCLED:		case P1_SELF_FGPLED:	case P1_EXUNIT_RLED:
		case P1_EXUNIT_GLED:	case P1_EXUNIT_CLED:	case P1_EXUNIT_GPLED:
		case P1_EXUNIT_FRLED:	case P1_EXUNIT_FGLED:	case P1_EXUNIT_FCLED:
		case P1_EXUNIT_FGPLED:	case P1_LCD_STATE:
			GetFromQue(P1FIFOIn, &ReceID);
			GetFromQue(P1FIFOIn, &Status1);
			GetFromQue(P1FIFOIn, &Status2);

			switch(Item) {
			case P1_EXUNIT_RLED:
			case P1_EXUNIT_GLED:
			case P1_EXUNIT_CLED:
			case P1_EXUNIT_GPLED:
			case P1_EXUNIT_FRLED:
			case P1_EXUNIT_FGLED:
			case P1_EXUNIT_FCLED:
			case P1_EXUNIT_FGPLED:
				P2TxCMD(ReceID, Item, Status1, Status2);
				break;
			}
			break;
		}
	} // end of if (GetFromQue(P1FIFOIn, &Item)
