class Bullet : public GameObject
{
public:

	CLASS_IDENTIFICATION( 'BULL', GameObject )

	enum EBulletReplicationState
	{
		EYRS_Pose			= 1 << 0,
		EYRS_Color			= 1 << 1,
		EYRS_PlayerId		= 1 << 2,

		EYRS_AllState	= EYRS_Pose | EYRS_Color | EYRS_PlayerId
	};

	static	GameObject*	StaticCreate() { return new Bullet(); }

	virtual uint32_t	GetAllStateMask()	const override	{ return EYRS_AllState; }

	virtual uint32_t	Write( OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState ) const override;

	void			SetVelocity( const Vector3& inVelocity )	{ mVelocity = inVelocity; }
	const Vector3&	GetVelocity() const					{ return mVelocity; }

	void		SetPlayerId( int inPlayerId )	{ mPlayerId = inPlayerId; }
	int			GetPlayerId() const				{ return mPlayerId; }

	void		InitFromShooter(Tank* inShooter );

	virtual void Update() override;

	virtual bool HandleCollisionWithTank(Tank* inWall ) override;
	bool       GetAsBullet() {return true;}
protected:
	Bullet();


	Vector3		mVelocity;

	float		mMuzzleSpeed;
	int			mPlayerId;

};

typedef shared_ptr< Bullet >	BulletPtr;