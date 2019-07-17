#include <TankWarClientPCH.h>

WallClient::WallClient()
{
	mSpriteComponent.reset( new SpriteComponent( this ) );
	mSpriteComponent->SetTexture( TextureManager::sInstance->GetTexture( "wall" ) );
}