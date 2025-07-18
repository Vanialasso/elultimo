#include <public/ccobsmng_iface_v1.h>

// ***********************************************************************

// class EDROOM_CTX_Top_0

// ***********************************************************************



	// CONSTRUCTORS***********************************************

CCObsMng::EDROOM_CTX_Top_0::EDROOM_CTX_Top_0(CCObsMng &act,
	 Pr_Time & EDROOMpVarVNextTimeOut ):

	EDROOMcomponent(act),
	Msg(EDROOMcomponent.Msg),
	MsgBack(EDROOMcomponent.MsgBack),
	Obsmng(EDROOMcomponent.Obsmng),
	ObservTimer(EDROOMcomponent.ObservTimer),
	AttCtrlTimer(EDROOMcomponent.AttCtrlTimer),
	CImageInterval(0,500000),
	VNextTimeOut(EDROOMpVarVNextTimeOut)
{
}

CCObsMng::EDROOM_CTX_Top_0::EDROOM_CTX_Top_0(EDROOM_CTX_Top_0 &context):

	EDROOMcomponent(context.EDROOMcomponent),
	Msg(context.Msg),
	MsgBack(context.MsgBack),
	Obsmng(context.Obsmng),
	ObservTimer(context.ObservTimer),
	AttCtrlTimer(context.AttCtrlTimer),
	CImageInterval(0,500000),
	VNextTimeOut(context.VNextTimeOut)
{

}

	// EDROOMSearchContextTrans********************************************

bool CCObsMng::EDROOM_CTX_Top_0::EDROOMSearchContextTrans(
			TEDROOMTransId &edroomCurrentTrans)
			{

	bool edroomValidMsg=false; 

	 switch(Msg->signal)
	{

		 case ( EDROOMSignalDestroy ): 

			 edroomValidMsg=true;
			 edroomCurrentTrans.distanceToContext = 0 ;
			 edroomCurrentTrans.localId = -1 ;
			 break;

	}

	return(edroomValidMsg);

}

	// User-defined Functions   ****************************

void	CCObsMng::EDROOM_CTX_Top_0::FDoAttitudeCtrl()

{

 pus_service129_do_attitude_ctrl();

}



void	CCObsMng::EDROOM_CTX_Top_0::FEndObservation()

{

VNextTimeOut.GetTime();
 pus_service129_end_observation();

}



void	CCObsMng::EDROOM_CTX_Top_0::FInit()

{
   //Define absolute time
  Pr_Time time;
	 
	//Timing Service useful methods
	 
	//time.GetTime(); // Get current monotonic time
	//time.Add(X,Y); // Add X sec + Y microsec
	 

time.GetTime();
VNextTimeOut+= Pr_Time(0,100000);
time=VNextTimeOut; 
   //Program absolute timer 
   ObservTimer.InformAt( time ); 
}



void	CCObsMng::EDROOM_CTX_Top_0::FProgAttitudeCtrl()

{
   //Define absolute time
  Pr_Time time;
	 
	//Timing Service useful methods
	 
	//time.GetTime(); // Get current monotonic time
	//time.Add(X,Y); // Add X sec + Y microsec
time.GetTime();	
VNextTimeOut+= Pr_Time(0,100000); // Add X sec + Y microsec 
time=VNextTimeOut; 

   //Program absolute timer 
   ObservTimer.InformAt( time ); 
}



void	CCObsMng::EDROOM_CTX_Top_0::FTakeImage()

{

 pus_service129_take_image();

}



void	CCObsMng::EDROOM_CTX_Top_0::FToObservation()

{

pus_service129_is_observation_ready();

}



bool	CCObsMng::EDROOM_CTX_Top_0::GLastImage()

{

return pus_service129_is_last_image();

}



bool	CCObsMng::EDROOM_CTX_Top_0::GReadyToObservation()

{

 return pus_service129_is_observation_ready();

}



void	CCObsMng::EDROOM_CTX_Top_0::FexecObs()

{
   //Handle Msg->data
  CDTCHandler & varSObsMng_TC = *(CDTCHandler *)Msg->data;
	
		// Data access
	
	// ... =varSObsMng_TC;
varSobsMng_TC.ExecObs

}



void	CCObsMng::EDROOM_CTX_Top_0::FProgTakeImage()

{


}



	//********************************** Pools *************************************



// ***********************************************************************

// class EDROOM_SUB_Top_0

// ***********************************************************************



	// CONSTRUCTOR*************************************************

CCObsMng::EDROOM_SUB_Top_0::EDROOM_SUB_Top_0 (CCObsMng&act):
		EDROOM_CTX_Top_0(act,
			VNextTimeOut)
{

}

	//***************************** EDROOMBehaviour ********************************

void CCObsMng::EDROOM_SUB_Top_0::EDROOMBehaviour()
{

	TEDROOMTransId edroomCurrentTrans;

	//Behaviour starts from Init State

	edroomCurrentTrans = EDROOMIArrival();

	do
	{

		//Take next transition

		switch(edroomCurrentTrans.localId)
		{

			//To Choice Point DoAttitudeCtrl
			case (DoAttitudeCtrl):

				//Execute Action 
				FDoAttitudeCtrl();
				//Evaluate Branch ToObservation
				if( GReadyToObservation() )
				{
					//Execute Action 
					FToObservation();

					//Branch taken is DoAttitudeCtrl_ToObservation
					edroomCurrentTrans.localId =
						DoAttitudeCtrl_ToObservation;

					//Next State is Observation
					edroomNextState = Observation;
				 } 
				//Default Branch ProgAttitudeCtrl
				else
				{
					//Execute Action 
					FProgAttitudeCtrl();

					//Branch taken is DoAttitudeCtrl_ProgAttitudeCtrl
					edroomCurrentTrans.localId =
						DoAttitudeCtrl_ProgAttitudeCtrl;

					//Next State is Standby
					edroomNextState = Standby;
				 } 
				break;
			//To Choice Point TakeImage
			case (TakeImage):

				//Execute Action 
				FTakeImage();
				//Evaluate Branch ObservationProg
				if( GLastImage() )
				{
					//Execute Actions 
					FEndObservation();
					FProgAttitudeCtrl();

					//Branch taken is TakeImage_ObservationProg
					edroomCurrentTrans.localId =
						TakeImage_ObservationProg;

					//Next State is Standby
					edroomNextState = Standby;
				 } 
				//Default Branch ProgTakeImage
				else
				{
					//Execute Action 
					FProgTakeImage();

					//Branch taken is TakeImage_ProgTakeImage
					edroomCurrentTrans.localId =
						TakeImage_ProgTakeImage;

					//Next State is Observation
					edroomNextState = Observation;
				 } 
				break;
			//Next Transition is Init
			case (Init):
				//Execute Action 
				FInit();
				//Next State is Standby
				edroomNextState = Standby;
				break;
			//Next Transition is ExecObs
			case (ExecObs):
				//Msg->Data Handling 
				FexecObs();
				//Next State is Standby
				edroomNextState = Standby;
				break;
		}

		//Entry into the Next State 
		switch(edroomNextState)
		{

				//Go to the state I
			case (I):
				//Arrival to state I
				edroomCurrentTrans=EDROOMIArrival();
				break;

				//Go to the state Standby
			case (Standby):
				//Arrival to state Standby
				edroomCurrentTrans=EDROOMStandbyArrival();
				break;

				//Go to the state Observation
			case (Observation):
				//Execute Entry Action 
				FProgTakeImage();
				//Arrival to state Observation
				edroomCurrentTrans=EDROOMObservationArrival();
				break;

		}

		edroomCurrentState=edroomNextState;

	}while(Msg->signal != EDROOMSignalDestroy);

}



	// Context Init**********************************************

void CCObsMng::EDROOM_SUB_Top_0::EDROOMInit()
{

edroomCurrentState=I;

}



//	 ***********************************************************************

//	 SubState I

//	 ***********************************************************************



TEDROOMTransId CCObsMng::EDROOM_SUB_Top_0::EDROOMIArrival()
{

	TEDROOMTransId edroomCurrentTrans;

	//Next transition is  Init
	edroomCurrentTrans.localId = Init;
	edroomCurrentTrans.distanceToContext = 0;
	return(edroomCurrentTrans);

}



	// ***********************************************************************

	// Leaf SubState  Standby

	// ***********************************************************************



TEDROOMTransId CCObsMng::EDROOM_SUB_Top_0::EDROOMStandbyArrival()
{

	TEDROOMTransId edroomCurrentTrans;

	bool edroomValidMsg=false;

	do
	{

		EDROOMNewMessage ();

		switch(Msg->signal)
		{

			case (EDROOMSignalTimeout): 

				 if (*Msg->GetPInterface() == AttCtrlTimer)
				{

					//Next transition is  DoAttitudeCtrl
					edroomCurrentTrans.localId = DoAttitudeCtrl;
					edroomCurrentTrans.distanceToContext = 0 ;
					edroomValidMsg=true;
				 }

				break;

			case (SObsMng_TC): 

				 if (*Msg->GetPInterface() == Obsmng)
				{

					//Next transition is  ExecObs
					edroomCurrentTrans.localId= ExecObs;
					edroomCurrentTrans.distanceToContext = 0;
					edroomValidMsg=true;
				 }

				break;

		};

		if (false == edroomValidMsg)
		{
			 edroomValidMsg = EDROOMSearchContextTrans(edroomCurrentTrans);

		}

	} while (false == edroomValidMsg);

	return(edroomCurrentTrans);

}



	// ***********************************************************************

	// Leaf SubState  Observation

	// ***********************************************************************



TEDROOMTransId CCObsMng::EDROOM_SUB_Top_0::EDROOMObservationArrival()
{

	TEDROOMTransId edroomCurrentTrans;

	bool edroomValidMsg=false;

	do
	{

		EDROOMNewMessage ();

		switch(Msg->signal)
		{

			case (EDROOMSignalTimeout): 

				 if (*Msg->GetPInterface() == ObservTimer)
				{

					//Next transition is  TakeImage
					edroomCurrentTrans.localId = TakeImage;
					edroomCurrentTrans.distanceToContext = 0 ;
					edroomValidMsg=true;
				 }

				break;

		};

		if (false == edroomValidMsg)
		{
			 edroomValidMsg = EDROOMSearchContextTrans(edroomCurrentTrans);

		}

	} while (false == edroomValidMsg);

	return(edroomCurrentTrans);

}



