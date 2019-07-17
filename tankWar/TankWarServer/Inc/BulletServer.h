class BulletServer : public Bullet
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn( new BulletServer() ); }
	void HandleDying() override;

	virtual bool		HandleCollisionWithTank(Tank* inCat ) override;

	virtual void Update() override;
	bool HandleCollisionWithMouse(Wall * inmouse);
protected:
	BulletServer();

private:
	float mTimeToDie;

};