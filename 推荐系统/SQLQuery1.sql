create database MovieLens
create table Users
(
	userID int primary key not null,
	age int
)

create table Items
(
	itemID int primary key not null,
    title char(5)
)

create table Ratings
(
	userID int not null,
	itemID int not null,
	rating int,
	FOREIGN KEY (userID) REFERENCES Users(userID),
	FOREIGN KEY (itemID) REFERENCES Items(itemID)
)

create table SimilarityMatrix
(
	itemOne int not null,
	itemTwo int not null,
	similarity float not null,
	FOREIGN KEY (itemOne) REFERENCES Items(itemID),
	FOREIGN KEY (itemTwo) REFERENCES Items(itemID)
)

declare @i int 
set @i=1
while @i<=6040
begin
insert into Users(userID) values(@i)
set @i=@i+1
end

declare @j int 
set @j=1
while @j<=3952
begin
insert into Items(itemID) values(@j)
set @j=@j+1
end

create table tb_test
(
	testID int not null
)

insert into tb_test values(12)