#include "Aquarium.h"
#include <cstdlib>


string AquariumCreatureTypeToString(AquariumCreatureType t){
    switch(t){
        case AquariumCreatureType::BiggerFish:
            return "BiggerFish";
        case AquariumCreatureType::NPCreature:
            return "BaseFish";
        case AquariumCreatureType::Octopus:
            return "Octopus";
        case AquariumCreatureType::Fish:
            return "Fish";
        default:
            return "UknownFish";
    }
}

// PlayerCreature Implementation
PlayerCreature::PlayerCreature(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: Creature(x, y, speed, 10.0f, 1, sprite) {}


void PlayerCreature::setDirection(float dx, float dy) {
    m_dx = dx;
    m_dy = dy;
    normalize();
}

void PlayerCreature::move() {
    m_x += m_dx * m_speed;
    m_y += m_dy * m_speed;
    this->bounce();
}

void PlayerCreature::reduceDamageDebounce() {
    if (m_damage_debounce > 0) {
        --m_damage_debounce;
    }
}

//This is a simple bump, this flips the direction and nudges away so the sprites separates.
void PlayerCreature::bump(){
    m_dx = -m_dx;
    m_dy = -m_dy;

    //moves the direction to avoid sticking
    m_x += m_dx * m_speed * 2.00;
    m_y += m_dy * m_speed * 2.00;
}

void PlayerCreature::update() {
    this->reduceDamageDebounce();
    this->move();
}

void PlayerCreature::draw() const {
    
    ofLogVerbose() << "PlayerCreature at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    if (this->m_damage_debounce > 0) {
        ofSetColor(ofColor::red); // Flash red if in damage debounce
    }
    if (m_sprite) {
        m_sprite->draw(m_x, m_y);
    }
    ofSetColor(ofColor::white); // Reset color

}

void PlayerCreature::changeSpeed(int speed) {
    m_speed = speed;
}

void PlayerCreature::loseLife(int debounce) {
    if (m_damage_debounce <= 0) {
        if (m_lives > 0) this->m_lives -= 1;
        m_damage_debounce = debounce; // Set debounce frames
        ofLogNotice() << "Player lost a life! Lives remaining: " << m_lives << std::endl;
    }
    // If in debounce period, do nothing
    if (m_damage_debounce > 0) {
        ofLogVerbose() << "Player is in damage debounce period. Frames left: " << m_damage_debounce << std::endl;
    }
}

// NPCreature Implementation
NPCreature::NPCreature(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: Creature(x, y, speed, 30, 1, sprite) {
    m_dx = (rand() % 3 - 1); // -1, 0, or 1
    m_dy = (rand() % 3 - 1); // -1, 0, or 1
    normalize();

    m_creatureType = AquariumCreatureType::NPCreature;
}

void NPCreature::move() {
    // Simple AI movement logic (random direction)
    m_x += m_dx * m_speed;
    m_y += m_dy * m_speed;
    if(m_dx < 0 ){
        this->m_sprite->setFlipped(true);
    }else {
        this->m_sprite->setFlipped(false);
    }
    bounce();
}

void NPCreature::draw() const {
    ofLogVerbose() << "NPCreature at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    ofSetColor(ofColor::white);
    if (m_sprite) {
        m_sprite->draw(m_x, m_y);
    }
}


BiggerFish::BiggerFish(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: NPCreature(x, y, speed, sprite) {
    m_dx = (rand() % 3 - 1);
    m_dy = (rand() % 3 - 1);
    normalize();

    setCollisionRadius(60); // Bigger fish have a larger collision radius
    m_value = 5; // Bigger fish have a higher value
    m_creatureType = AquariumCreatureType::BiggerFish;
}

void BiggerFish::move() {
    // Bigger fish might move slower or have different logic
    m_x += m_dx * (m_speed * 0.5); // Moves at half speed
    m_y += m_dy * (m_speed * 0.5);
    if(m_dx < 0 ){
        this->m_sprite->setFlipped(true);
    }else {
        this->m_sprite->setFlipped(false);
    }

    bounce();
}

void BiggerFish::draw() const {
    ofLogVerbose() << "BiggerFish at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    this->m_sprite->draw(this->m_x, this->m_y);
}

// Octopus

Octopus::Octopus(float x, float y, int speed, std::shared_ptr<GameSprite> sprite) 
: NPCreature (x, y, speed, sprite) {
    m_dx = (rand() % 3 - 1);
    m_dy = (rand() % 3 - 1);
    normalize();

    setCollisionRadius(70);
    m_value = 8;
    m_creatureType = AquariumCreatureType:: Octopus;
}

// Move

void Octopus::move(){
    
    // diagonal move 
    if (m_dx == 0 || m_dy == 0) {
        do {
            m_dx = (rand() % 3 - 1); 
            m_dy = (rand() % 3 - 1);
        } while (m_dx == 0 || m_dy == 0); 
        normalize();
    }

    // speed increase
    m_x += m_dx * (m_speed + 15);
    m_y += m_dy * (m_speed + 15);

   
    bounce();

    // Flip
    if (m_dx < 0)
        m_sprite->setFlipped(true);
    else
        m_sprite->setFlipped(false);
}

// DrAW

void Octopus:: draw() const {
    ofLogVerbose() << "Octopuse at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    this->m_sprite->draw(this->m_x, this->m_y);
} 

//Fish implementation
Fish::Fish(float x, float y, int speed, std::shared_ptr<GameSprite>sprite)
: NPCreature(x, y, speed, sprite){
    m_dx = (rand() % 3) - 1;
    m_dy = (rand() % 3) - 1;
    normalize();

    setCollisionRadius(30); //to make the fish smaller than the octopus
    m_value = 2;   //weaker than the octupus
    m_creatureType = AquariumCreatureType::NPCreature;
}

void Fish::move(){
    m_x += m_dx * (m_speed + 2);  //a bit quicker and lighter
    m_y += m_dy * (m_speed + 2);

    if(m_dx < 0) this->m_sprite->setFlipped(true);
    else{
        this->m_sprite->setFlipped(false);

        bounce(); //this is the wall bounce from creature bounce
    }
}

void Fish::draw() const{
    ofLogVerbose() << "Fish at (" << m_x << ", " << m_y << ") speed " << m_speed << std::endl;
    m_sprite->draw(m_x, m_y);
}

// AquariumSpriteManager
AquariumSpriteManager::AquariumSpriteManager(){
    this->m_npc_fish = std::make_shared<GameSprite>("base-fish.png", 70,70);
    this->m_big_fish = std::make_shared<GameSprite>("bigger-fish.png", 120, 120);
    this->m_octopus = std::make_shared<GameSprite>("pokemonX.png", 100, 100);
    this->m_shark = std::make_shared<GameSprite>("shark.png", 120,120);
    this->m_fish = std::make_shared<GameSprite>("fish.png", 90, 80);

}

std::shared_ptr<GameSprite> AquariumSpriteManager::GetSprite(AquariumCreatureType t){
    switch(t){
        case AquariumCreatureType::BiggerFish:
            return std::make_shared<GameSprite>(*this->m_big_fish);
            
        case AquariumCreatureType::NPCreature:
            return std::make_shared<GameSprite>(*this->m_npc_fish);
        case AquariumCreatureType::Octopus:
            return std::make_shared<GameSprite>(*this->m_octopus);
        case AquariumCreatureType::Shark:
            return std::make_shared<GameSprite>(*this->m_shark);
        case AquariumCreatureType::Fish:
            return std::make_shared<GameSprite>(*this->m_fish);
        default:
            return nullptr;
    }
}


// Aquarium Implementation
Aquarium::Aquarium(int width, int height, std::shared_ptr<AquariumSpriteManager> spriteManager)
    : m_width(width), m_height(height) {
        m_sprite_manager =  spriteManager;
    }



void Aquarium::addCreature(std::shared_ptr<Creature> creature) {
    creature->setBounds(m_width - 20, m_height - 20);
    m_creatures.push_back(creature);
}

void Aquarium::addAquariumLevel(std::shared_ptr<AquariumLevel> level){
    if(level == nullptr){return;} // guard to not add noise
    this->m_aquariumlevels.push_back(level);
}

void Aquarium::update() {
    for (auto& creature : m_creatures) {
        creature->move();
    }
    this->Repopulate();
}

void Aquarium::draw() const {
    for (const auto& creature : m_creatures) {
        creature->draw();
    }
}


void Aquarium::removeCreature(std::shared_ptr<Creature> creature) {
    auto it = std::find(m_creatures.begin(), m_creatures.end(), creature);
    if (it != m_creatures.end()) {
        ofLogVerbose() << "removing creature " << endl;
        int selectLvl = this->currentLevel % this->m_aquariumlevels.size();
        auto npcCreature = std::static_pointer_cast<NPCreature>(creature);
        this->m_aquariumlevels.at(selectLvl)->ConsumePopulation(npcCreature->GetType(), npcCreature->getValue());
        m_creatures.erase(it);
    }
}

void Aquarium::clearCreatures() {
    m_creatures.clear();
}

std::shared_ptr<Creature> Aquarium::getCreatureAt(int index) {
    if (index < 0 || size_t(index) >= m_creatures.size()) {
        return nullptr;
    }
    return m_creatures[index];
}



void Aquarium::SpawnCreature(AquariumCreatureType type) {
    int x = rand() % this->getWidth();
    int y = rand() % this->getHeight();
    int speed = 1 + rand() % 25; // Speed between 1 and 25

    switch (type) {
        case AquariumCreatureType::NPCreature:
            this->addCreature(std::make_shared<NPCreature>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::NPCreature)));
            break;
        case AquariumCreatureType::BiggerFish:
            this->addCreature(std::make_shared<BiggerFish>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::BiggerFish)));
            break;
        case AquariumCreatureType::Octopus:
            this->addCreature(std::make_shared<Octopus>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::Octopus)));
            break;
        case AquariumCreatureType::Fish:
            this->addCreature(std::make_shared<Fish>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::Fish)));
            break;
        default:
            ofLogError() << "Unknown creature type to spawn!";
            break;
    }

}


// repopulation will be called from the levl class
// it will compose into aquarium so eating eats frm the pool of NPCs in the lvl class
// once lvl criteria met, we move to new lvl through inner signal asking for new lvl
// which will mean incrementing the buffer and pointing to a new lvl index
void Aquarium::Repopulate() {
    ofLogVerbose("entering phase repopulation");
    // lets make the levels circular
    int selectedLevelIdx = this->currentLevel % this->m_aquariumlevels.size();
    ofLogVerbose() << "the current index: " << selectedLevelIdx << endl;
    std::shared_ptr<AquariumLevel> level = this->m_aquariumlevels.at(selectedLevelIdx);


    if(level->isCompleted()){
        level->levelReset();
        this->currentLevel += 1;
        selectedLevelIdx = this->currentLevel % this->m_aquariumlevels.size();
        ofLogNotice()<<"new level reached : " << selectedLevelIdx << std::endl;
        level = this->m_aquariumlevels.at(selectedLevelIdx);
        this->clearCreatures();
    }

    
    // now lets find how many to respawn if needed 
    std::vector<AquariumCreatureType> toRespawn = level->Repopulate();
    ofLogVerbose() << "amount to repopulate : " << toRespawn.size() << endl;
    if(toRespawn.size() <= 0 ){return;} // there is nothing for me to do here
    for(AquariumCreatureType newCreatureType : toRespawn){
        this->SpawnCreature(newCreatureType);
    }
}


// Aquarium collision detection
std::shared_ptr<GameEvent> DetectAquariumCollisions(std::shared_ptr<Aquarium> aquarium, std::shared_ptr<PlayerCreature> player) {
    if (!aquarium || !player) return nullptr;
    
    for (int i = 0; i < aquarium->getCreatureCount(); ++i) {
        auto creature = aquarium->getCreatureAt(i);
        if(creature && creature != player && checkCollision(player, creature)){
            return std::make_shared<GameEvent>(GameEventType::COLLISION, player, creature);
        }
    }
    return nullptr;
};

void AquariumGameScene::Update() {
    std::shared_ptr<GameEvent> event;
    m_player->update();

static bool playerTransformed = false; 
    if (!playerTransformed && m_player->getScore() >= 50) {
        auto shark = std::make_shared<Shark>(*m_player,m_aquarium->GetSpriteManager()->GetSprite(AquariumCreatureType::Shark));

        m_player = shark;  
        playerTransformed = true;
        ofLogNotice() << "Player transformed into Shark!" << std::endl;
    }

    if (updateControl.tick()) {
        // find a collision between player and any creature
        event = DetectAquariumCollisions(m_aquarium, m_player);

        if (event && event->isCollisionEvent()) {
            ofLogVerbose() << "Collision detected between player and NPC!" << std::endl;

            if (event->creatureB) {
                event->print();

                // If player is weaker -> take damage and bounce back
                if (m_player->getPower() < event->creatureB->getValue()) {
                    ofLogNotice() << "Player is too weak to eat the creature!" << std::endl;

                    m_player->loseLife(3 * 60);  // 3 seconds
                   
                    if (m_player->getLives() <= 0) {
                        m_lastEvent = std::make_shared<GameEvent>(
                            GameEventType::GAME_OVER, m_player, nullptr);
                        return;
                    }
                } else {
                    // Player strong enough -> eat it and score
                    m_aquarium->removeCreature(event->creatureB);
                    m_player->addToScore(1, event->creatureB->getValue());

                    if (m_player->getScore() % 25 == 0) {
                        m_player->increasePower(1);
                        ofLogNotice() << "Player power increased to "
                                      << m_player->getPower() << "!" << std::endl;
                    }
                }
            } else {
                ofLogError() << "Error: creatureB is null in collision event." << std::endl;
            }
        }


        m_aquarium->update();
    }
}


void AquariumGameScene::Draw() {
    this->m_player->draw();
    this->m_aquarium->draw();
    this->paintAquariumHUD();

}


void AquariumGameScene::paintAquariumHUD(){
    float panelWidth = ofGetWindowWidth() - 150;
    ofDrawBitmapString("Score: " + std::to_string(this->m_player->getScore()), panelWidth, 20);
    ofDrawBitmapString("Power: " + std::to_string(this->m_player->getPower()), panelWidth, 30);
    ofDrawBitmapString("Lives: " + std::to_string(this->m_player->getLives()), panelWidth, 40);
    for (int i = 0; i < this->m_player->getLives(); ++i) {
        ofSetColor(ofColor::red);
        ofDrawCircle(panelWidth + i * 20, 50, 5);
    }
    ofSetColor(ofColor::white); // Reset color to white for other drawings
}

void AquariumLevel::populationReset(){
    for(auto node: this->m_levelPopulation){
        node->currentPopulation = 0; // need to reset the population to ensure they are made a new in the next level
    }
}

void AquariumLevel::ConsumePopulation(AquariumCreatureType creatureType, int power){
    for(std::shared_ptr<AquariumLevelPopulationNode> node: this->m_levelPopulation){
        ofLogVerbose() << "consuming from this level creatures" << endl;
        if(node->creatureType == creatureType){
            ofLogVerbose() << "-cosuming from type: " << AquariumCreatureTypeToString(node->creatureType) <<" , currPop: " << node->currentPopulation << endl;
            if(node->currentPopulation == 0){
                return;
            } 
            node->currentPopulation -= 1;
            ofLogVerbose() << "+cosuming from type: " << AquariumCreatureTypeToString(node->creatureType) <<" , currPop: " << node->currentPopulation << endl;
            this->m_level_score += power;
            return;
        }
    }
}

bool AquariumLevel::isCompleted(){
    return this->m_level_score >= this->m_targetScore;
}


