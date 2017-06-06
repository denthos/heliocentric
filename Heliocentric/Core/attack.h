#pragma once
#include <ctime>

/**
A class for defining a certain attack.

Examples include:
laser attack
missile attack
lifesteal attack

Attacks should have the following:
Range
Damage (As of now, damage applies instantly.. no DoTs)
Cooldown (The longer the cooldown, the slower the attack is)
ProjectileSpeed 

On the client side, attacks should also be bound to a DrawableAttack, which
will define how it is drawn.
*/

class AttackableGameObject;

class Attack {
private:

	enum AttackState {
		READY,
		PROJECTILE_TRAVELLING,
		DAMAGING,
		COOLDOWN
	};

	std::clock_t begin_action_time;
	bool hasWaitedLength(float length) const;

	AttackState state;
	float range;
	float projectile_delay_ms;
	float damage;
	float cooldown_ms;

	void changeState(AttackState newState);

	void beginAttackProcedure();
	virtual void doProjectileTravel();
	virtual void doDamage(AttackableGameObject* target);
	virtual void doCooldown();

public:
	Attack() {}
	Attack(float range, float projectile_delay_ms, float damage, float cooldown_ms);
	float getRange() const;
	float getDamage() const;

	bool projectileInMotion() const;
	bool damaging() const;

	/**
	Attacks the target. This does attack logic instantly, so the
	caller better make sure that the target is in range
	@return Whether the attack is attacking
	*/
	void doAttack(AttackableGameObject* target);
};