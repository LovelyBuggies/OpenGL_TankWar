#include <TankWarServerPCH.h>


BulletServer::BulletServer()
{
	//yarn lives a second...
	mTimeToDie = Timing::sInstance.GetFrameStartTime() + 1.f;
}

void BulletServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject( this );
}


void BulletServer::Update()
{
	Bullet::Update();

	if( Timing::sInstance.GetFrameStartTime() > mTimeToDie )
	{
		SetDoesWantToDie( true );
	}

}

bool BulletServer::HandleCollisionWithTank(Tank* inCat )
{
	if( inCat->GetPlayerId() != GetPlayerId() )
	{
		//kill yourself!
		SetDoesWantToDie( true );

		static_cast<TankServer* >( inCat )->TakeDamage( GetPlayerId() );

	}

	return false;
}
bool BulletServer::HandleCollisionWithMouse(Wall* inMouse )
{
	
		//kill yourself!
		SetDoesWantToDie( true );
		inMouse->SetDoesWantToDie(true);


	return false;
}

