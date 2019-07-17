#include <TankWarServerPCH.h>


WallServer::WallServer()
{
}

void WallServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject( this );
}


bool WallServer::HandleCollisionWithTank(Tank* inWall )
{
	//kill yourself!
	SetDoesWantToDie( false );

	//ScoreBoardManager::sInstance->IncScore( inCat->GetPlayerId(), 1 );

	return false;
}


