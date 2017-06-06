#include "attack.h"
#include "attackable_game_object.h"
#include "logging.h"

Attack::Attack(float range, float projectile_delay_ms, float damage, float cooldown_ms) :
	range(range), projectile_delay_ms(projectile_delay_ms), damage(damage), cooldown_ms(cooldown_ms),
	state(READY) {}


bool Attack::projectileInMotion() const {
	return (state == PROJECTILE_TRAVELLING);
}

bool Attack::damaging() const {
	return (state == DAMAGING);
}


float Attack::getRange() const {
	return this->range;
}


float Attack::getDamage() const {
	return this->damage;
}

bool Attack::hasWaitedLength(float length) const {
	auto elapsed_time = std::clock() - begin_action_time;
	return (elapsed_time >= length);
}

void Attack::changeState(AttackState newState) {
	begin_action_time = std::clock();
	state = newState;
}

void Attack::beginAttackProcedure() {
	/* LAUNCH THE PROJECTILE! */
	changeState(PROJECTILE_TRAVELLING);
}

void Attack::doProjectileTravel() {
	/* Wait until the projectile's delay applies */
	if (hasWaitedLength(projectile_delay_ms)) {
		changeState(DAMAGING);
	}
}

void Attack::doDamage(AttackableGameObject* target) {
	/* Let's apply instantaneous damage */
	target->take_damage((int) damage);
	changeState(COOLDOWN);
}

void Attack::doCooldown() {
	if (hasWaitedLength(cooldown_ms)) {
		changeState(READY);
	}
}

void Attack::doAttack(AttackableGameObject* target) {
	if (!target) {
		LOG_ERR("Cannot attack invalid target (nullptr)");
		return;
	}

	switch (state) {
	case READY:
		beginAttackProcedure();
		break;
	case PROJECTILE_TRAVELLING:
		doProjectileTravel();
		break;
	case DAMAGING:
		doDamage(target);
		break;
	case COOLDOWN:
		doCooldown();
		break;
	}
}