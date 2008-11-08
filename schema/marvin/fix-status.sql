-- change status of meta data to A (Active)

update  mtcl
set     mtcl__status = 'A';
update  mtat
set     mtat__status = 'A';
update  mtfn
set     mtfn__status = 'A';
update  mtrl
set     mtrl__status = 'A';

alter table mtcl
alter column mtcl__status  SET DEFAULT 'A';

alter table mtat
alter column mtat__status  SET DEFAULT 'A';

alter table mtfn
alter column mtfn__status  SET DEFAULT 'A';

alter table mtrl
alter column mtrl__status  SET DEFAULT 'A';

update mtat set mtat_input_method = '' where mtat_name ~ 'mt.._.*' and mtat_input_method ~ 'FSL=';
