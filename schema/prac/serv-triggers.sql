-- -*- sql -*-
 
--    $Id$

--  Copyright 1996 X=X Computer Software Trust
--   		  Kangaroo Ground Australia 3097


-- This is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public Licensepublished by
-- the Free Software Foundation; either version 2, or (at your option)
-- any later version.
 
-- This software is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
 
-- You should have received a copy of the GNU General Public License
-- along with this software; see the file COPYING.  If not, write to
-- the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 
-- Report problems and direct all questions to:
 
--     Rex McMaster, rmcm@compsoft.com.au
 

 
-- Revision Log
  
--
-- 
--
--
-- PL function to set defaults if null
--
drop trigger serv_tr_before on serv;
drop function serv_tr_before();
create function serv_tr_before()
returns trigger as $$
   DECLARE

    BEGIN
    -- set a dummy code
    if ( new.serv_code is null ) then
      new.serv_code = 'NEW-'||new.serv__sequence::text;
    end if;

    if ( new.serv_desc is null ) then
      new.serv_desc = 'no description';
    end if;

    return new;

    END; $$
    LANGUAGE 'plpgsql';

create trigger serv_tr_before before insert or update
    on serv for each row
    execute procedure serv_tr_before();

--
-- PL function to check before delete
--
drop trigger tr_serv_ckdel on serv;
drop function fn_serv_ckdel();
create function fn_serv_ckdel()
returns trigger as $$
    DECLARE
      tmp_svpf_count integer;
    BEGIN
    -- check for dependents
   
    select into tmp_svpf_count count(*)
    from   svpf
    where  svpf_serv_code = old.serv_code;
    
    if ( TG_OP = 'DELETE' ) then
       if tmp_svpf_count > 0  then
         RAISE EXCEPTION 'The Service Code % has % dependent services recorded, so cannot be deleted',
                old.serv_code, tmp_svpf_count;
       end if;
       return old;
    end if;

    if ( TG_OP = 'UPDATE' ) then
       if ( tmp_svpf_count > 0  and new.serv_code <> old.serv_code ) then
         RAISE EXCEPTION 'The Service Code % has % dependent services recorded, so cannot be changed',
                old.serv_code, tmp_svpf_count;
       end if;
    end if;

    return new;
    END; $$
    LANGUAGE 'plpgsql';

create trigger tr_serv_ckdel before delete or update
    on serv for each row
    execute procedure fn_serv_ckdel();
