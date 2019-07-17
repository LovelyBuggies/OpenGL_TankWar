class WallClient : public Wall
{
public:
	static	GameObjectPtr	StaticCreate()		{ return GameObjectPtr( new WallClient() ); }

protected:
	WallClient();

private:

	SpriteComponentPtr	mSpriteComponent;
};