CREATE TABLE character
(
    character_id INT PRIMARY KEY,
    name VARCHAR(255),
    model_id INT
);

CREATE TABLE hitbox
(
    hitbox_id INT PRIMARY KEY,
    filepath VARCHAR(255)
);

create TABLE move
(
    move_id INT PRIMARY KEY,
    hitbox_id INT,
    hitbox_start INT,
    hitbox_end INT,
    hitbox_scale INT
);