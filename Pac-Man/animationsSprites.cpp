#include "animationsSprites.h"    


Explosion::Explosion()
{
	_sonsExplosion.add("explosion", "explosion.wav");
	_explosionTextureComplet.loadFromFile("explosion.png");

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			_explosionTextureRect[i][j] = sf::IntRect(i * 256, j * 256, 256, 256);
			_explosionTexture[i][j].setSize(sf::Vector2f(256, 256));
			_explosionTexture[i][j].setTexture(&_explosionTextureComplet);
			_explosionTexture[i][j].setTextureRect(_explosionTextureRect[i][j]);
		}
	}
}

void Explosion::playSonExplosion()
{
	_sonsExplosion.play("explosion");
}
Explosion::~Explosion()
{

}

void Explosion::setPos(sf::Vector2f pos)
{
	_posExplosion = pos;
}

void Explosion::drawExplosion(sf::RenderTarget &rt)
{
	if (_sonsExplosion.isPlaying("explosion"))
	{
		if (!_flagExplosion)
		{
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					_explosionTexture[i][j].setPosition(_posExplosion);
					_explosionTexture[i][j].setOrigin(128, 128);
				}
			}

			_flagExplosion = true;
		}
		rt.draw(_explosionTexture[_explosionAnimation / 8][_explosionAnimation % 8]);
		_vitesseExplosion++;
		if (_vitesseExplosion >= 7)
		{
			incrementerAnimationExplosion();
			_vitesseExplosion = 0;
		}

	}
	else
		_flagExplosion = false;
}
void Explosion::incrementerAnimationExplosion()
{
	_explosionAnimation++;
	if (_explosionAnimation > 47)
	{
		_explosionAnimation = 0;
	}
}
