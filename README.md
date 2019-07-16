# Tank War 

<p align="center">
<img src="http://ww2.sinaimg.cn/large/006tNc79ly1g4ym4sws67j318a0osgqc.jpg" alt="ScoreChain" title="ScoreChain" length = "1400" width="500"/><br/>
</p>
<p align="center">
<a href="https://gitee.com/code_sysu/syllabus/blob/master/Lectures/PAOGD%E8%AE%BA%E6%96%87%E5%88%86%E4%BA%AB%E9%A1%BA%E5%BA%8F.xlsx"><img src="https://img.shields.io/badge/team-17-aa3333.svg"></a>
<a href="https://www.youtube.com/watch?v=itYok4eUv3A"><img src="https://img.shields.io/badge/theme-tank war-00aace.svg"></a>
<a href="https://gitee.com/paogdfenzu10/GroupProject"><img src="https://img.shields.io/badge/direction-sever-yellow.svg"></a>
<a href="https://gitee.com/paogdfenzu10/GroupProject"><img src="https://img.shields.io/badge/coverage-A-blue.svg"></a>
<br/>
<a href="hhttps://gitee.com/paogdfenzu10/GroupProject"><img src="https://img.shields.io/appveyor/ci/gruntjs/grunt.svg"></a>
<a href="https://gitee.com/paogdfenzu10/GroupProject"><img src="https://img.shields.io/badge/platform-Win 10-a7bE6E.svg"></a>
<a href="https://gitee.com/paogdfenzu10/GroupProject"><img src="https://img.shields.io/badge/date-June~July, 2019-orange.svg"></a>
<a href="https://996.icu"><img src="https://img.shields.io/badge/link-996.icu-8855dd.svg" alt="996.icu"></a>
<br/><br/>
</p>
---

## I. 游戏简介

我们组设计的网络多人游戏《坦克大战》，以二战坦克为题材，既保留了射击类游戏的操作性，也改进了射击类游戏太过于复杂难玩的高门槛特点，是一款集休闲与竞技于一身的游戏。提供二战知名坦克供您选择，各坦克独立个性强化系统，更拥有独特皮肤闪亮登场，让每一个玩家都能找到适合自己的归属。我们还将推出故事背景与真实战役，以及万众期待的火炮系统，传说与梦想中的坦克，一切都值得您的到来。

---

## II. 游戏玩法

- WASD控制坦克的移动，K发射子弹；
- 游戏开始，各玩家进入场景，拥有不同的皮肤；
- 一个玩家有3次生命。被击中三次后，玩家死亡，重新进入场景；
- 坦克和发射的子弹无法穿过墙壁，必须直面其他炮火。

---

## III. 设计目标

- 完成坦克和墙壁等背景渲染，初步描绘游戏场景；
- 完成坦克大战游戏的框架搭建，实现基本的游戏操作；
- 支持多玩家比赛、信息同步；
- 实现低延时、解决抖动问题。

---

## IV. 运行说明

运行此游戏需要您从我们的仓库下载和安装该游戏，然后在Windows 10系统上进行部署。

### 一. 安装说明

安装步骤如下:

```shell
$ git clone https://gitee.com/paogdfenzu10/GroupProject.git
$ cd ./GroupProject/code/不知道起什么名字/tankWar
```

### 二. 部署说明

Win10系统中，在Visual Studio2017（*VS2019可能存在版本受限*）中打开项目目录，打开`./TankWar.sln`文件，重新生成解决方案。然后进入`./Debug`目录下，`Shift+右键`打开PowShell，进行以下操作：

```shell
$ cmd
$ TankWarSever 50000 
$ TankWarClient 127.0.0.1:50000 Alice //也可以用您自己服务器的端口
```

如果您是游戏的发起者，那么您需要执行上面的操作；如果您是参与的玩家，只需运行客户端即可。

---

## V. 游戏构思

我们的项目服务端主要分为三个模块：客户端-服务器网络交互模块、客户端代理模块和游戏世界状态处理模块。

- 客户端-服务器网络交互模块主要的功能是设计网络协议（UDP）和接口（数据包格式），这里分别在服务器和客户端实现，两部分都要实现数据包的接收处理和发送功能；
- 客户端代理要处理存储从网络交互模块获取的玩家各种指令（客户端接入，移动等），然后提供相应接口给游戏世界状态处理模块用于获取客户端对应的游戏对象，并且将处理后的结果返回给网络交互模块；
- 游戏世界状态处理模块主要的功能是处理从网络交互模块获取的各种指令，对世界状态做相应的改变（游戏对象的创建、销毁、移动等），同时要实现将游戏世界复制给客户端代理用于给客户端更新

---

## VI. 实现过程 

![](https://i.loli.net/2019/07/15/5d2c320e7561914654.png)

### 一. 功能实现
以下简述项目的三个模块的实现过程：

#### 1. 客户端-服务器网络交互

这部分工作主要是在NetworkManagerClient和NetworkManagerServer这两个类中完成的，NetworkManagerClient负责发送连接请求和客户端的指令，并接收处理服务端的数据包，NetworkMangerServer负责接收处理客户端的指令并返回相应的状态信息。

网络交互主要有以下几个功能：

- 客户端请求加入游戏，服务器记录并分配ID给客户端，同时创建游戏对象

- 客户端发送玩家指令给服务端，服务器处理

- 服务器将游戏世界中游戏对象的复制信息发送给客户端，客户端处理

- 客户端断开连接，服务器处理

以下介绍实现思路：

首先要实现UDP协议，用于后面发送数据包调用。这里声明一个UDPSocket类，该类有三个主要方法：Bind、SendTo和ReceiveFrom，分别实现绑定地址、发送和接收数据包功能：

```c++
class UDPSocket
{
public:

	~UDPSocket();

	int Bind( const SocketAddress& inToAddress );
	int SendTo( const void* inToSend, int inLength, const SocketAddress& inToAddress );
	int ReceiveFrom( void* inToReceive, int inMaxLength, SocketAddress& outFromAddress );

private:
	friend class SocketUtil;
	UDPSocket( SOCKET inSocket ) : mSocket( inSocket ) {}
	SOCKET mSocket;
};
```

**（1）新客户端加入游戏**

新客户端要加入游戏，首先向服务器发送“hello”数据包，数据包内包含数据包类型（供服务端识别）和玩家名字：

```c++
void NetworkManagerClient::SendHelloPacket()
{
	OutputMemoryBitStream helloPacket; 
	//写入数据包类型和玩家名字
	helloPacket.Write( kHelloCC );
	helloPacket.Write( mName );

	SendPacket( helloPacket, mServerAddress );
}
```

服务端接收到“hello”数据包后，便给新玩家分配ID，并创建新的客户端代理记录下相关信息，然后发送“welcome”数据包给客户端：

```c++
void NetworkManagerServer::HandlePacketFromNewClient( InputMemoryBitStream& inInputStream, const SocketAddress& inFromAddress )
{
	uint32_t	packetType;
	inInputStream.Read( packetType );
	//判断是否为"hello"数据包
	if(  packetType == kHelloCC )
	{
		string name;
		//读取数据包内玩家名字并创建新的客户端代理用于记录
		inInputStream.Read( name );
		ClientProxyPtr newClientProxy = std::make_shared< ClientProxy >( inFromAddress, name, mNewPlayerId++ );
		mAddressToClientMap[ inFromAddress ] = newClientProxy;
		mPlayerIdToClientMap[ newClientProxy->GetPlayerId() ] = newClientProxy;
        //在游戏世界中创建一辆坦克
		static_cast< Server* > ( Engine::sInstance.get() )->HandleNewClient( newClientProxy );

		//发送"welcome"数据包
		SendWelcomePacket( newClientProxy );

		//在游戏世界中加入该客户端的游戏对象
		for( const auto& pair: mNetworkIdToGameObjectMap )
		{
			newClientProxy->GetReplicationManagerServer().ReplicateCreate( pair.first, pair.second->GetAllStateMask() );
		}
	}
	else
	{
		LOG( "Bad incoming packet from unknown client at socket %s", inFromAddress.ToString().c_str() );
	}
}
//设置数据包类型
void NetworkManagerServer::SendWelcomePacket( ClientProxyPtr inClientProxy )
{
	OutputMemoryBitStream welcomePacket; 

	welcomePacket.Write( kWelcomeCC );
	welcomePacket.Write( inClientProxy->GetPlayerId() );

	LOG( "Server Welcoming, new client '%s' as player %d", inClientProxy->GetName().c_str(), inClientProxy->GetPlayerId() );

	SendPacket( welcomePacket, inClientProxy->GetSocketAddress() );
}
```

客户端接收"welcome"数据包后读取记录自己的ID，并设置自身状态（已加入游戏）：

```c++
void NetworkManagerClient::HandleWelcomePacket(
InputMemoryBitStream& inInputStream )
{
	if( mState == NCS_SayingHello )
	{
		//if we got a player id, we've been welcomed!
		int playerId;
		inInputStream.Read( playerId );
		mPlayerId = playerId;
		mState = NCS_Welcomed;
		LOG( "'%s' was welcomed on client as player %d", mName.c_str(), mPlayerId );
	}
}

```

**（2）客户端发送玩家指令，服务器处理**

客户端的NetworkManagerClient获取指令队列，设置好数据包格式后发送给服务器：

```c++
void NetworkManagerClient::SendInputPacket()
{
	//获取玩家指令
	MoveList& moveList = InputManager::sInstance->GetMoveList();

	if( moveList.HasMoves() )
	{
		OutputMemoryBitStream inputPacket; 
		inputPacket.Write( kInputCC );  //设置数据包类型
         int moveCount = moveList.GetMoveCount();
		int startIndex = moveCount > 3 ? moveCount - 3 - 1 : 0;
		inputPacket.Write( moveCount - startIndex, 2 );
		for( int i = startIndex; i < moveCount; ++i )
		{
			moveList[i].Write( inputPacket );
		}
		//发送指令数据包
		SendPacket( inputPacket, mServerAddress );
		moveList.Clear();
	}
}
```

服务器该部分的工作是判断是玩家指令数据包后，将其存入到客户端代理供服务器其他部分处理（后面会介绍）：

```c++
void NetworkManagerServer::HandleInputPacket( ClientProxyPtr inClientProxy, InputMemoryBitStream& inInputStream )
{
	uint32_t moveCount = 0;
	Move move;
	inInputStream.Read( moveCount, 2 );
	
	for( ; moveCount > 0; --moveCount )
	{
		if( move.Read( inInputStream ) )
		{
			if( inClientProxy->GetUnprocessedMoveList().AddMove( move ) )
			{
				inClientProxy->SetIsLastMoveTimestampDirty( true );
			}
		}
	}
}
```

**（3）服务器将游戏对象的复制信息发送给客户端，客户端处理**

服务端设置数据包类型后，将游戏对象的复制信息加入到数据包中发送给客户端：

```c++
void NetworkManagerServer::SendStatePacketToClient( ClientProxyPtr inClientProxy )
{
	OutputMemoryBitStream	statePacket;
	//数据包类型
	statePacket.Write( kStateCC );
	WriteLastMoveTimestampIfDirty( statePacket, inClientProxy );
    //游戏面板信息（比如生命值，得分等）
	AddScoreBoardStateToPacket( statePacket );
	//游戏对象状态信息
	inClientProxy->GetReplicationManagerServer().Write( statePacket );
	SendPacket( statePacket, inClientProxy->GetSocketAddress() );
}
```

客户端接收到数据包后便进行解析，读取其中的时间戳、游戏面板数值以及游戏对象复制信息等：

```c++
void NetworkManagerClient::HandleStatePacket( InputMemoryBitStream& inInputStream )
{
	if( mState == NCS_Welcomed )
	{
		ReadLastMoveProcessedOnServerTimestamp( inInputStream );
		HandleScoreBoardState( inInputStream );
		mReplicationManagerClient.Read( inInputStream );
	}
}
```

**（4）客户端断开，服务器处理**

服务器检验客户端最后发送数据包的时间间隔，若超时未发送新数据包给服务器说明该客户端已断开，此时便删除该客户端的地址信息及ID，并将其从游戏世界中删除：

```c++
void NetworkManagerServer::CheckForDisconnects()
{
	vector< ClientProxyPtr > clientsToDC;
	float minAllowedLastPacketFromClientTime = Timing::sInstance.GetTimef() - mClientDisconnectTimeout;
    //检验每一个客户端最后发送数据包的时间，若超过规定时间没有发送则移除该客户端相关数据
	for( const auto& pair: mAddressToClientMap )
	{
		if( pair.second->GetLastPacketFromClientTime() < minAllowedLastPacketFromClientTime )
		{
			clientsToDC.push_back( pair.second );
		}
	}
	for( ClientProxyPtr client: clientsToDC )
	{
		HandleClientDisconnected( client );
	}
}
//清除该客户端代理对应的地址信息及客户端ID，并将其从游戏世界删除
void NetworkManagerServer::HandleClientDisconnected( ClientProxyPtr inClientProxy )
{
	mPlayerIdToClientMap.erase( inClientProxy->GetPlayerId() );
	mAddressToClientMap.erase( inClientProxy->GetSocketAddress() );
	static_cast< Server* > ( Engine::sInstance.get() )->HandleLostClient( inClientProxy );

	if( mAddressToClientMap.empty() )
	{
		Engine::sInstance->SetShouldKeepRunning( false );
	}
}
```

以上便是客户端与服务器的网络交互的实现。



#### 2. 客户端代理

为了方便降低服务端处理世界状态模块处理客户端指令的难度，这里我们采用了用客户端代理存储网络交互模块从客户端获取的指令。该类中包含了客户端的地址名字ID等信息，这样服务端处理世界状态模块只要从中获取所需信息便可对其对应游戏对象进行操作，同时将处理后的结果返回到该类中，最后网络交互模块也只要将该类对象发送给客户端即可，从而降低了两个模块的工作难度。

核心代码：

```c++
class ClientProxy
{
public:
	ClientProxy( const SocketAddress& inSocketAddress, const string& inName, int inPlayerId );
	const SocketAddress& GetSocketAddress() const { return mSocketAddress; }//获取客户端地址
	int	GetPlayerId() const	{ return mPlayerId; }//获取玩家ID
	const string& GetName() const { return mName; }//获取玩家名字
    void UpdateLastPacketTime();//更新客户端发送最后一个数据包的时间
	float GetLastPacketFromClientTime()	const { return mLastPacketFromClientTime; }//获取客户端发送最后一个数据包的时间
    ReplicationManagerServer& GetReplicationManagerServer() { return mReplicationManagerServer; }//获取对象复制信息存储类
    const MoveList& GetUnprocessedMoveList() const { return mUnprocessedMoveList; }//获取未处理指令
    MoveList& GetUnprocessedMoveList() { return mUnprocessedMoveList; }//获取已处理指令
    void HandleTankDied();//处理坦克死亡重生

private:
	ReplicationManagerServer mReplicationManagerServer;
	SocketAddress mSocketAddress;
	string mName;
	int mPlayerId;
	float mLastPacketFromClientTime;
	MoveList mUnprocessedMoveList;
};
```



#### 3. 服务端处理游戏世界状态

**（1）游戏对象的创建和销毁**

以坦克为例，定义一个函数HandleNewClient，该函数在NetworkMangerServer中接收到新客户端的连接请求时（HandlePacketFromNewClient函数中）调用，以创建新的玩家坦克。根据传入的客户端代理inClientProxy，获取其ID创建新坦克并加入到游戏世界中：

```c++
//给NetworkMangerServer类调用
void Server::HandleNewClient( ClientProxyPtr inClientProxy )
{
	int playerId = inClientProxy->GetPlayerId();
	ScoreBoardManager::sInstance->AddEntry( playerId, inClientProxy->GetName() );
	SpawnTankForPlayer( playerId );
}
//在游戏世界中加入新坦克
void Server::SpawnTankForPlayer( int inPlayerId )
{
	TankPtr tank = std::static_pointer_cast<Tank>( GameObjectRegistry::sInstance->CreateGameObject( 'TANK' ) );
	tank->SetColor( ScoreBoardManager::sInstance->GetEntry( inPlayerId )->GetColor() );
	tank->SetPlayerId( inPlayerId );
	tank->SetLocation( Vector3( 1.f - static_cast< float >( inPlayerId ), 0.f, 0.f ) );
}
```

定义HandleLostClient函数，将该客户端代理对应的客户端从分数板中移除，并销毁坦克：

```c++
void Server::HandleLostClient( ClientProxyPtr inClientProxy )
{
	int playerId = inClientProxy->GetPlayerId();
    //将该客户端从分数板中移除，并销毁坦克
	ScoreBoardManager::sInstance->RemoveEntry( playerId );
	TankPtr tank = GetTankForPlayer( playerId );
	if( tank )
	{
		tank->SetDoesWantToDie( true );
	}
}
```

**（2）复制世界状态**

我们实现客户端世界状态同步的方法是在服务器创建世界，然后将其发送给客户端，客户端只负责渲染。由于我们的游戏世界并不大，因此可以考虑将整个世界状态装入一个数据包。这里把每次经过处理后的游戏世界状态写入流后，供网络交互模块调用发送给客户端，从而实现各客户端世界状态的同步。

核心代码：

```c++
void ReplicationManagerServer::Write( OutputMemoryBitStream& inOutputStream )
{
	//遍历游戏世界
	for( auto& pair: mNetworkIdToReplicationCommand )
	{
		ReplicationCommand& replicationCommand = pair.second;
		if( replicationCommand.HasDirtyState() )
		{
			int networkId = pair.first;
             //先将游戏对象ID写入数据流
			inOutputStream.Write( networkId );
             //获取该对象要执行的动作
			ReplicationAction action = replicationCommand.GetAction();
			inOutputStream.Write( action, 2 );
			uint32_t writtenState = 0;
			uint32_t dirtyState = replicationCommand.GetDirtyState();
			//根据获取到的动作（创建、更新、销毁等），分别执行不同的写入函数
			switch( action )
			{
			case RA_Create:
				writtenState = WriteCreateAction( inOutputStream, networkId, dirtyState );
				replicationCommand.SetAction( RA_Update );
				break;
			case RA_Update:
				writtenState = WriteUpdateAction( inOutputStream, networkId, dirtyState );
				break;
			case RA_Destroy:
				writtenState = WriteDestroyAction( inOutputStream, networkId, dirtyState );
				mNetworkIdsToRemove.emplace_back( networkId );
				break;
			}
			replicationCommand.ClearDirtyState( writtenState );
		}
	}
```

**（3）更新游戏世界状态**

这里分为三个部分，分别是坦克、子弹和墙的处理过程，具体对应TankServer、BulletServer和WallServer这三个类。这里以坦克位列，介绍一下我们的实现思路。

首先是实现坦克要进行的相关操作，主要是处理自身销毁、发射子弹和受到伤害这三个功能

```c++
//客户端退出游戏则销毁该游戏对象
void TankServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject( this );
}
//处理发射子弹的相关工作（如创建子弹）
void TankServer::HandleShooting()
{
	float time = Timing::sInstance.GetFrameStartTime();
	if( mIsShooting && Timing::sInstance.GetFrameStartTime() > mTimeOfNextShot )
	{
		mTimeOfNextShot = time + mTimeBetweenShots;

		BulletPtr bullet = std::static_pointer_cast<Bullet>( GameObjectRegistry::sInstance->CreateGameObject( 'BULL' ) );
		bullet->InitFromShooter( this );
	}
}

void TankServer::TakeDamage( int inDamagingPlayerId )
{
	mHealth--;
    //判断生命值是否小于0，如果是，执行死亡和重生
	if( mHealth <= 0.f )
	{
		//计分板变化
		ScoreBoardManager::sInstance->IncScore( inDamagingPlayerId, 1 );
         //设置死亡
		SetDoesWantToDie( true );
		//重生，通知客户端代理更新
		ClientProxyPtr clientProxy = NetworkManagerServer::sInstance->GetClientProxy( GetPlayerId() );
		if( clientProxy )
		{
			clientProxy->HandleTankDied();
		}
	}
	//重置生命值
	NetworkManagerServer::sInstance->SetStateDirty( GetNetworkId(), ECRS_Health );
}
```

接着便在update函数里执行从客户端代理获取的动作队列，执行坦克的移动转向以及是否发射子弹等操作：

```c++
void TankServer::Update()
{
	Tank::Update();
	
	Vector3 oldLocation = GetLocation();
	Vector3 oldVelocity = GetVelocity();
	float oldRotation = GetRotation();

	ClientProxyPtr client = NetworkManagerServer::sInstance->GetClientProxy( GetPlayerId() );
	if( client )
	{
		MoveList& moveList = client->GetUnprocessedMoveList();
		for( const Move& unprocessedMove : moveList )
		{
			const InputState& currentState = unprocessedMove.GetInputState();
			float deltaTime = unprocessedMove.GetDeltaTime();
			ProcessInput( deltaTime, currentState );
			SimulateMovement( deltaTime );
		}
		moveList.Clear();
	}

	HandleShooting();

	if( !TankMath::Is2DVectorEqual( oldLocation, GetLocation() ) ||
		!TankMath::Is2DVectorEqual( oldVelocity, GetVelocity() ) ||
		oldRotation != GetRotation() )
	{
		NetworkManagerServer::sInstance->SetStateDirty( GetNetworkId(), ECRS_Pose );
	}
}
```

### 二. 文件架构
接着介绍一下我们的项目文件架构以及各文件实现的工作。项目文件主要分为五类：Objects、Client Proxy、Manager Sever、ServerPCH和Server

- Objects包含了Bullet Server、Wall Server和Tank Server这三部分，分别管理子弹、墙和坦克在游戏中的行为；
- Client Proxy是客户端代理，负责网络交互与游戏世界处理的信息交流；
- Manger Server包含复制管理服务器和网络管理服务器；
- ServerPCH负责整合所需头文件；
- Server部分负责整合服务器的整体功能。

#### 1. Objects

游戏主要涉及三种对象，分别是子弹、墙、坦克。每个种类都有不同的属性，我们在其基类上分别添加了它们的服务器，用来管理它们在游戏中的行为。

##### 1.1. Bullet Server

Bullet Sever继承了Bullet类。实现细节如下：

- 属性
    - `mTimeToDie`：子弹消失的时间，是子弹消失1s之后消失；
- 方法
    - `StaticCreate()`：创建一颗子弹；
    - `Update()`如果超过了子弹生命周期，子弹消失，并更新画面；
    - `HandleCollisionWithTank(Tank* inTank)`：当子弹碰到其他坦克之后，子弹消失，被击中的玩家承受伤害；
    - `HandleCollisionWithWall(Wall* inWall)`：当子弹击中子弹墙之后，子弹消失。

##### 1.2. Wall Server

Wall Sever继承了Wall类。实现细节如下：

- 方法
    - `StaticCreate()`：创建墙壁。

##### 1.3. Tank Server

Tank Sever继承了Tank类。Tank的控制者可以从`ECatControlType`中进行选择，包括真人玩家或者AI控制。这个设计允许在参与人数不够的情况下，自动坦克的参与，增加了游戏趣味和可扩展性。

- 属性
    - `mCatControlType`：坦克的控制者——真人orAI；
    - `mTimeOfNextShot`：因为坦克不能过于频繁的射击子弹（*不然游戏会不公平*），所以需要定义最早可以的下次射击的时间；
    - `mTimeBetweenShots`：坦克两次射击之间的时间间隔（*间隔0.2s才可以发射下一颗子弹*）；
- 方法
    - `StaticCreate()`：创建坦克；
    - `HandleShooting()`：处理射击，如果发射子弹的间隔时间超过了0.2s，那么允许发射子弹；
    - `HandleDying()`：处理毁灭，当坦克被击中毁灭时，坦克服务器自动让坦克下线；
    - `Update()`：坦克更新，包括以下几部分：
        - 获取旧的坦克信息，包括位置、速度、角度；
        - 每间隔`deltaTime`与客户端交互一次，同步信息；
        - 根据HandleShooting()`方法处理射击；
        - 通过网络连接进行通信，完成数据的传输；
    - `TakeDamage()`：用来进承受伤害的控制，如果生命值小于0，设置坦克下线，否则生命值减1。

#### 2. Client Proxy

客户端代理负责服务端和客户端的交互，主要支持对四种交流功能：包信息的交换、获得未处理的Movelist、设置Movelist的DirtyState、以及坦克的毁灭与重生。

- 属性
    - `mSocketAddress`：socket地址；
    - `mName`：客户端名字；
    - `mPlayerId`：玩家的ID
    - `mLastPacketFromClientTime`：最后一个传递的包；
    - `mTimeToRespawn`：下次重生的时间；
    - `mUnprocessedMoveList`：尚未处理的MoveList；
    - `mIsLastMoveTimestampDirty`：是否设定了DirtyState；
- 方法
    - `Get\*()`：各类获取函数；
    - `UpdateLastPacketTime()`：更新最后一个包时间；
    - `SetIsLastMoveTimestampDirty( bool inIsDirty )`：设置DirtyState；
    - `IsLastMoveTimestampDirty()`：是否设置过了DirtyState；
    - `HandleTankDied()`：处理坦克毁灭，这里的处理包括让坦克在`kRespawnDelay`后重生；
    - `ClientProxy::RespawnCatIfNecessary()`：创建新的坦克实例。

#### 3. Manager Sever

管理服务器包括两个：复制管理服务器和网络管理服务器。其中，复制管理服务器用批量复制到机制保证了数据传输的可靠性；而网络管理服务器提供了客户端与服务端的网络连接，保证了两者交互的可能性，并在此实现了对象的注册和下线。以下是它们的详细介绍。

##### 3.1. Replication Manager Server

Replication Manager Server继承了Replication Manager类，是它的服务器，支持批量复制游戏中的请求——对于每一个对象都可以批量地创建、更新和删除所有属性。Replication Manager服务器的实现主要分为以下几个模块：

- 属性
    - `mNetworkIdToReplicationCommand`：需要复制请求信息的网络ID；
    - `mNetworkIdsToRemove`：需要删除请求信息的网络ID；
- 方法
    - `WriteCreateAction( OutputMemoryBitStream& inOutputStream, int inNetworkId, uint32_t inDirtyState )`：将一个游戏对象写入网络请求ID的包；
    - `WriteUpdateAction( OutputMemoryBitStream& inOutputStream, int inNetworkId, uint32_t inDirtyState )`：更新网络请求ID包中的游戏对象；
    - `WriteDestroyAction( OutputMemoryBitStream& inOutputStream, int inNetworkId, uint32_t inDirtyState )`：删除网络请求ID包中的游戏对象；
    - `ReplicateCreate( int inNetworkId, uint32_t inInitialDirtyState )`：建立对象复制，网络请求ID的复制命令根据复制命令的DirtyState确定；
    - `ReplicateDestroy( int inNetworkId )`：销毁网络请求ID的复制命令；
    - `RemoveFromReplication( int inNetworkId )`：将网络请求ID从需要复制命令的名单中除名；
    - `SetStateDirty( int inNetworkId, uint32_t inDirtyState )`：设置DirtyState，决定着网络请求的复制命令；
    - `Write( OutputMemoryBitStream& inOutputStream )`：写入复制命令是这个服务器的核心，主要包括以下几个部分：
        - 首先基于DirtyState，确定需要写入的复制命令是什么；
        - 然后明确处理的目标对象，也就是NetworkId；
        - 然后根据功能需要，创建、更新和删除动作；
        - 最后清除DirtyState。

ReplicationManagerTransmissionData是TransmissionData的继承类。当ReplicationManagerServer发现了一个带有批量命令的网络标识符，就会创建一个ReplicationManagerTransmissionData来保证传输可靠数据。其中包括一个小类——ReplicationTransmission（包含网络ID、动作和状态信息）。具体实现如下：

- 属性
    - `mReplicationManagerServer`：mReplicationManagerServer的实例；
    - `mTransmissions`：vector容器，存储ReplicationTransmission的实例；
- 方法
    - `AddTransmission( int inNetworkId, ReplicationAction inAction, uint32_t inState )`：增加ReplicationTransmission，包含网络ID、动作和状态信息；
    - `HandleDeliveryFailure( DeliveryNotificationManager* inDeliveryNotificationManager )`：处理网络ID的失败传输，有创建、更新和销毁的失败，分别传到下面的方法中处理；
        - `HandleCreateDeliveryFailure`：如果对象存在，重发创建传输；
        - `HandleDestroyDeliveryFailure`：重发销毁传输；
        - `HandleUpdateStateDeliveryFailure`：如果对象存在，重发更新传输；从Dirty State中删除写状态；
    - `HandleDeliverySuccess( DeliveryNotificationManager* inDeliveryNotificationManager )`：处理成功的传输，进行传输，如果有任何传输被破坏，那么从映射中删除这个网络ID。

##### 3.2. Network Manager Sever

Network Manager服务器继承了NetworkManager类，提供了客户端与服务端的网络连接，支持两者之间的传包，并允许游戏中新对象的加入。实现的细节如下：

- 属性
    - `sInstance`：网络管理服务器实例；
    - `mAddressToClientMap`：客户端的身份；
    - `ToClientMapmPlayerIdToClientMap`：客户端的地址；
    - `mNewPlayerId`：新玩家的ID；
    - `mNewNetworkId`：新网络的ID；
    - `mTimeOfLastSatePacket`：上次传包的时间；
    - `mTimeBetweenStatePackets`：传包允许的时间；
    - `mClientDisconnectTimeout`：未连接到间隔；
- 方法
    - `RegisterAndReturn( GameObject* inGameObject )`：创建新的对象；
    - `ProcessPacket( InputMemoryBitStream& inInputStream, const SocketAddress& inFromAddress )`：处理包，包括寻找客户端代理、根据包分类（*有hello、input、和无效三种类型*）进行不同的处理；
    - `HandleConnectionReset( const SocketAddress& inFromAddress )`：重新设置连接；
    - `SendOutgoingPackets()`：向客户端发送一个状态包；
    - `CheckForDisconnects()`：检查是否连接中断了；
    - `RegisterGameObject( GameObjectPtr inGameObject )`：注册一个新的对象；
    - `UnregisterGameObject( GameObject* inGameObject )`：让一个对象下线；
    - `SetStateDirty( int inNetworkId, uint32_t inDirtyState )`：对某个网络ID设置DirtyState；
    - `RespawnTanks()`：让坦克复活；
    - `GetClientProxy( int inPlayerId )`：给玩家找网络代理。

#### 4. ServerPCH

PHC服务器整合所有需要的头文件，使代码结构更加有条理。整合需要的Inc包有：`TankWarShared.h`、`ReplicationManagerServer.h`、`ClientProxy.h`、`NetworkManagerServer.h`、`Server.h`、`TankServer.h`、`WallServer.h`、`BulletServer.h`。


#### 5. Server

整合服务器继承了基类中的Engine类，包括以下功能：初始化网络功能、创建游戏世界、处理客户端事宜、为每个玩家分配坦克。具体实现如下：

- 方法
    - `Server()`：构造函数提供了服务器的初始化操作，包括以下功能模块：
        - 实例化坦克、子弹、墙壁；
        - 初始化网络管理器；
        - 设置延迟；
    - `InitNetworkManager()`：初始化网络管理器；
    - `SetupWorld()`：创建游戏世界，包括创造地图和对象的实例；
    - `DoFrame()`：定义了每一帧的行动，包括处理数据包、检查连接性、让坦克复活以及发送数据包；
    - `HandleNewClient( ClientProxyPtr inClientProxy )`：创建新的代理服务器；
    - `SpawnTankForPlayer( int inPlayerId )`：给玩家分配坦克；
    - `HandleLostClient( ClientProxyPtr inClientProxy )`：处理丢失的服务器代理；
    - `GetTankForPlayer( int inPlayerId )`：获取玩家的坦克；
    - - `Run()`：开始游戏。

---

## VII. 问题与解决

- 《坦克大战》这个游戏涉及多个模块的交互，因此我们必须对彼此的设计了解透彻。刘硕曾想要尝试想要换成TCP框架，但林俊浩已经写完了基于UDP传输的服务端；刘硕曾想要尝试为数据包增加`isCreatedSuccess`、`isUpdatedSuccess`、`isDestroyedSuccess`等属性，但林锦竹已经完成了网络交互模块的构建；“多个独立进程之间很难同步”。
- 《坦克大战》网络多人游戏，一个主要的难点体现在**网络**。这次的难点在于我们还要增加很多依附功能来提升游戏玩家体验，我们用网络模块管理、延迟抖动创建了可靠的UDP数据传输。
- 虽然课堂有讲过很多理论知识，但是具体到实现时，很多编码技术我们还不熟练（*开发ReplicationTransmission时，一开始完全不知道怎么做，后来参考了书中的讲解以及查阅了很多成熟项目的代码才逐渐完成*）。
- 数据包的处理问题。一开始我没有给数据包分类型，并且没有对客户端的网络状态作标记，导致客户端和服务器的网络处理部分显得没有条理，几乎所有的过程都必须堆在同一个函数里。后来给数据包分了类型，这样客户端和服务器在接收到数据包时首先判断数据包类型再决定要执行的处理过程，分层次对数据包处理这样代码便更有条理易于维护；并且给客户端做了标记，客户端在接收到数据包时便清楚要读取什么内容（例如处于未接入状态时应读取是否为“welcome”数据包及分配到的ID）。
- UDP乱序及连接问题。由于UDP协议发送数据包可能会出现乱序的问题，为了解决这个问题我一开始的思路是给数据包加上编号，服务器接收到数据包时进行判断这样便可有效避免乱序，但由于服务器和客户端发送数据包比较频繁，数据包太多了编号有点困难。后来我的解决方案是在数据包中加入时间戳，可根据数据包发送时间进行判断，并且这样还方便记录客户端最后发送数据包的时间，可判断客户端是否已断开连接。

- 因为数据以流的形式进行传输，所以在对数据进行操作时会报一些类型错误和内存错误
解决方法：对数据进行强制转换，一开始使用的是 （type*）类型的转换，有时候会出错，后来使用`static_cast< dataType>` 对数据进行转换后就没有此类错误

---

## VIII. 效果展示

《坦克大战》的玩家将会拥有良好的玩家体验，我们将我们的游戏demo放到了Youtube（[视频传输门](https://www.youtube.com/watch?v=itYok4eUv3A)）。

![](http://ww3.sinaimg.cn/large/006tNc79ly1g4ukrbnb9jg30rs0egx6q.gif)

![](http://ww4.sinaimg.cn/large/006tNc79ly1g4ukr9mxf7g30rs0egb2b.gif)

![](http://ww1.sinaimg.cn/large/006tNc79ly1g4ukr77t9lg30rs0egb2c.gif)

---

## IX. 参考资料

1. [《网络多人游戏架构与编程》](https://book.douban.com/subject/27135506/)

---