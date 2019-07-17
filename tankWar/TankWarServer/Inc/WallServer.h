class WallServer : public Wall
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn( new WallServer() ); }
	void HandleDying() override;
	virtual bool		HandleCollisionWithTank(Tank* inCat ) override;

protected:
	WallServer();

};