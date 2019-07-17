enum ECatControlType
{
	ESCT_Human,
	ESCT_AI
};

class TankServer : public Tank
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn( new TankServer() ); }
	virtual void HandleDying() override;

	virtual void Update() override;

	void SetCatControlType( ECatControlType inCatControlType ) { mCatControlType = inCatControlType; }

	void TakeDamage( int inDamagingPlayerId );

protected:
	TankServer();

private:

	void HandleShooting();

	ECatControlType	mCatControlType;


	float		mTimeOfNextShot;
	float		mTimeBetweenShots;

};