create table login (
    username varchar(8) not null,
    pwd varchar(128) not null,
    constraint login_pk primary key (username)
);

create table session (
    session_id serial not null,
    username varchar(8) not null,
    expiration timestamp not null,
    creation timestamp not null,
    constraint session_pkey primary key (session_id),
    constraint username_fkey foreign key (username) references login(username)
);