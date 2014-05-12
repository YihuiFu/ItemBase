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
select * from Ratings

create index Ratings_Index
on Ratings(userID,itemID)

create table SimilarityMatrix
(
	itemOne int not null,
	itemTwo int not null,
	similarity float not null,
	FOREIGN KEY (itemOne) REFERENCES Items(itemID),
	FOREIGN KEY (itemTwo) REFERENCES Items(itemID)
)

create index SimilarityMatrix_Index
on SimilarityMatrix(itemOne)

create table UserInterest
(
	userID int not null,
	itemID int not null,
	interest float not null
	FOREIGN KEY (userID) REFERENCES Users(userID),
	FOREIGN KEY (itemID) REFERENCES Items(itemID)	
)

create index UserInterest_Index
on UserInterest(userID)

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

select top(10) itemID,COUNT(itemID) as total from Ratings group by itemID order by total desc

create table tb_TestMatrix
(
	itemOne int not null,
	itemTwo int not null,
	similarity float not null
)

create table tb_TestRatings
(
	userID int not null,
	itemID int not null,
	rating int
)

select count(*) from SimilarityMatrix
DELETE FROM tb_TestRatings where userID>0

select * from tb_TestMatrix