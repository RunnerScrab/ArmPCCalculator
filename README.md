# Arm PC Calculator
## Usage
Select guild and subguild to view skill and ability information. The effects of overlapping skills will be listed
below the skill sheet preview.

## Database
The file `armskills.db` is a SQLite db file, which means you can view and edit it with any SQLite db file viewer,
such as [sqlitebrowser](https://sqlitebrowser.org/). You will also be able to query the db file with sql statements.

For example, to list all classes who get parry:
```
select class_name from class_skills where skill_name = 'parry';

"Fighter"
"Soldier"
"Laborer"
"Raider"
"Scout"
"Stalker"
"Adventurer"
"Enforcer"
"Infiltrator"
"Miscreant"
"Protector"
"Swordsman"
```
