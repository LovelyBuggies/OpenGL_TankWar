class TankWarClient : public Tank
{
public:
	static	GameObjectPtr	StaticCreate()		{ return GameObjectPtr( new TankWarClient() ); }

	virtual void Update();
	virtual void	HandleDying() override;

	virtual void	Read( InputMemoryBitStream& inInputStream ) override;

protected:
	TankWarClient();


private:
	float				mTimeLocationBecameOutOfSync;
	float				mTimeVelocityBecameOutOfSync;
	
	SpriteComponentPtr	mSpriteComponent;
};