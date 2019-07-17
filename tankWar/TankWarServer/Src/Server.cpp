
#include <TankWarServerPCH.h>
#include<iostream>



//uncomment this when you begin working on the server

bool Server::StaticInit()
{
	sInstance.reset( new Server() );

	return true;
}

Server::Server()
{

	GameObjectRegistry::sInstance->RegisterCreationFunction( 'TANK', TankServer::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'WALL', WallServer::StaticCreate );
	GameObjectRegistry::sInstance->RegisterCreationFunction( 'BULL', BulletServer::StaticCreate );

	InitNetworkManager();
	
	// Setup latency
	float latency = 0.0f;
	string latencyString = StringUtils::GetCommandLineArg( 2 );
	if( !latencyString.empty() )
	{
		latency = stof( latencyString );
	}
	NetworkManagerServer::sInstance->SetSimulatedLatency( latency );
}


int Server::Run()
{
	SetupWorld();

	return Engine::Run();
}

bool Server::InitNetworkManager()
{
	string portString = StringUtils::GetCommandLineArg( 1 );
	uint16_t port = stoi( portString );

	return NetworkManagerServer::StaticInit( port );
}


namespace
{
	
	void CreateRandomMice( int inMouseCount )
	{
		Vector3 mouseMin( -5.f, -3.f, 0.f );
		Vector3 mouseMax( 5.f, 3.f, 0.f );
		GameObjectPtr go;

		//make a mouse somewhere- where will these come from?
		for( int i = 0; i < inMouseCount; ++i )
		{
			go = GameObjectRegistry::sInstance->CreateGameObject( 'WALL' );
			Vector3 mouseLocation = TankMath::GetRandomVector( mouseMin, mouseMax );
			go->SetLocation( mouseLocation );
		}
	}
	void CreateWall(Vector3 Loc,int row,int column){
		float x = Loc.mX;
		float y = Loc.mY;
		float tempx,tempy;
		GameObjectPtr go;
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < column; j++)
			{
				tempx = x + 0.5*i;
				tempy = y + 0.6*j;
				if(tempx >= -5 && tempx <= 5 && tempy >= -3 && tempy <= 3)
				{
					
					go = GameObjectRegistry::sInstance->CreateGameObject('WALL');
					Vector3 mouseLocation = Vector3(tempx,tempy, 0.f);
					go->SetLocation(mouseLocation);
				}
			}
		}
	}
	void CreateMap(){
		GameObjectPtr go;
		float x = -5.f;
		float y = -3.f;
		Vector3 loc1(-5.f,-2.f,0.f);
		Vector3 loc2(-3.f,-2.f,0.f);
		Vector3 loc3(-5.f,1.f,0.f);
		Vector3 loc4(-3.f,2.f,0.f);
		Vector3 loc5(-2.5,2.3,0.f);
		Vector3 loc6(-2.f,2.6,0.f);
		Vector3 loc7(1.5,2.3,0.f);
		Vector3 loc8(2.f,2.0,0.f);
		Vector3 loc9(1.f,1.f,0.f);
		Vector3 loc10(2.f,-2.f,0.f);
		Vector3 loc11(4.f,-2.f,0.f);

		Vector3 loc12(-4.f, -3.5, 0.f);
		Vector3 loc13(-1.5, 0.5, 0.f);
		Vector3 loc14(1.5, -2.5, 0.f);
		Vector3 loc15(4.f, -0.5, 0.f);

		CreateWall(loc12,1,6);
		CreateWall(loc13,1,6);
		CreateWall(loc14,1,6); 
		CreateWall(loc15,1,6);

	}


}


void Server::SetupWorld()
{
	//spawn some random mice
	// CreateRandomMice( 10 );
	
	//spawn more random mice!
	// CreateRandomMice( 10 );
	CreateMap();
}

void Server::DoFrame()
{
	NetworkManagerServer::sInstance->ProcessIncomingPackets();

	NetworkManagerServer::sInstance->CheckForDisconnects();

	NetworkManagerServer::sInstance->RespawnCats();

	Engine::DoFrame();

	NetworkManagerServer::sInstance->SendOutgoingPackets();

}

void Server::HandleNewClient( ClientProxyPtr inClientProxy )
{
	
	int playerId = inClientProxy->GetPlayerId();
	
	ScoreBoardManager::sInstance->AddEntry( playerId, inClientProxy->GetName() );
	SpawnCatForPlayer( playerId );
}

void Server::SpawnCatForPlayer( int inPlayerId )
{
	RoboCatPtr cat = std::static_pointer_cast<Tank>( GameObjectRegistry::sInstance->CreateGameObject( 'TANK' ) );
	cat->SetColor( ScoreBoardManager::sInstance->GetEntry( inPlayerId )->GetColor() );
	cat->SetPlayerId( inPlayerId );
	//gotta pick a better spawn location than this...
	cat->SetLocation( Vector3( 1.f - static_cast< float >( inPlayerId ), 0.f, 0.f ) );

}

void Server::HandleLostClient( ClientProxyPtr inClientProxy )
{
	//kill client's cat
	//remove client from scoreboard
	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->RemoveEntry( playerId );
	RoboCatPtr cat = GetCatForPlayer( playerId );
	if( cat )
	{
		cat->SetDoesWantToDie( true );
	}
}

RoboCatPtr Server::GetCatForPlayer( int inPlayerId )
{
	//run through the objects till we find the cat...
	//it would be nice if we kept a pointer to the cat on the clientproxy
	//but then we'd have to clean it up when the cat died, etc.
	//this will work for now until it's a perf issue
	const auto& gameObjects = World::sInstance->GetGameObjects();
	for( int i = 0, c = gameObjects.size(); i < c; ++i )
	{
		GameObjectPtr go = gameObjects[ i ];
		Tank* cat = go->GetAsWall();
		if( cat && cat->GetPlayerId() == inPlayerId )
		{
			return std::static_pointer_cast<Tank>( go );
		}
	}

	return nullptr;

}