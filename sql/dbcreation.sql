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
    username varchar(16) not null,
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

create table module_status (
    module_id int not null,
    status boolean not null,
    constraint module_status_pkey primary key (module_id)
);

insert into module_status values
    (1, true),
    (2, true),
    (3, true),
    (4, true),
    (5, true),
    (6, true),
    (7, true),
    (8, true),
    (9, true),
    (10, true);

-- type 1 is system file, 2 is binary
create table monitor (
    monitor_id serial not null,
    fullfilename text not null,
    type int not null,
    active boolean not null,
    constraint monitor_pkey primary key (monitor_id)
);

-- type 1 is sniffer
create table targeted_proc (
    targeted_proc_id serial not null,
    name text not null,
    type int not null,
    active boolean not null,
    constraint targeted_proc_pkey primary key (targeted_proc_id)
);