create table "role" (
    "role" varchar(16) not null,
    constraint role_pkey primary key ("role")
);

insert into "role" values
    ('admin'), ('user'), ('spectator');

create table login (
    username varchar(16) not null,
    pwd varchar(128) not null,
    "role" varchar(16) not null,
    constraint login_pkey primary key (username),
    constraint role_fkey foreign key ("role") references "role"("role")
);

insert into login values ('admin',
    '$6$0nLTuzHmB9pJSGFn$KrLO2VIj7ZFhlAtyUnDLgY0nZvjCq1UVZnO8cvZrgvOxp9mxZdS9UDM5AmkWogJMX11pXifQ9KV5dak7dVlW//',
    'admin'),
    ('user', '$6$0nLTuzHmB9pJSGFn$KrLO2VIj7ZFhlAtyUnDLgY0nZvjCq1UVZnO8cvZrgvOxp9mxZdS9UDM5AmkWogJMX11pXifQ9KV5dak7dVlW//',
    'user'),
    ('spectator', '$6$0nLTuzHmB9pJSGFn$KrLO2VIj7ZFhlAtyUnDLgY0nZvjCq1UVZnO8cvZrgvOxp9mxZdS9UDM5AmkWogJMX11pXifQ9KV5dak7dVlW//',
    'spectator');

create table session (
    session_id serial not null,
    username varchar(8) not null,
    expiration timestamp,
    creation timestamp not null,
    constraint session_pkey primary key (session_id),
    constraint username_fkey foreign key (username) references login(username)
);

create table alarm (
    alarm_id int not null,
    severity int not null,
    "description" varchar(512),
    constraint alarm_pkey primary key (alarm_id)
);