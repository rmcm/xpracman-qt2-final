--  -*- sql -*-
--  
--    $Id$

--  Copyright 2001 X=X Computer Software Trust
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
 

-- 
-- Create a function to insert via view
-- This function returns the serial_number of the new "virtual" row.
-- This is used instead of an "insert" rule, as the latter cannot use "lastoid"
-- to revover the inserted row.
-- 
drop function crep_insert_row();
create function crep_insert_row()
returns integer as $$
    DECLARE
                rec_crep crep%ROWTYPE;
                tmp_cred__sequence cred.cred__sequence%TYPE; 
                rec_invc invc%ROWTYPE;
                rec_cred cred%ROWTYPE;
                rec_paym paym%ROWTYPE;
                
    BEGIN

         -- Do not know invoice yet, so use 0 
         if ( rec_crep.crep_invc__sequence is null ) then
           -- new invoice = 0 to trigger
           rec_crep.crep_invc__sequence := 0;
            select  *
            into    rec_invc
            from    invc
            where   invc__sequence = rec_crep.crep_invc__sequence;
            -- if not found create a dummy to temporarily satisfy constraints
            if ( not found ) then
              insert into invc(invc__sequence) values(0);
            end if;
         end if;

         -- Do not know payment yet, so use 0 
         if ( rec_crep.crep_paym__sequence is null ) then
           -- new payment = 0 to trigger
           rec_crep.crep_paym__sequence := 0;
            select  *
            into    rec_paym
            from    paym
            where   paym__sequence = rec_crep.crep_paym__sequence;
            -- if not found create a dummy to temporarily satisfy constraints
            if ( not found ) then
              insert into paym(paym__sequence) values(0);
            end if;
         end if;

         -- set to non-null
         if ( rec_crep.crep_cred_amount is null ) then
            rec_crep.crep_cred_amount := 0.0;
         end if;

         -- set to non-null
         if ( rec_crep.crep_cred_gst_amount is null ) then
            rec_crep.crep_cred_gst_amount := 0.0;
         end if;


         -- get a cred__sequence
         select nextval('cred_cred__sequence_seq')
         into tmp_cred__sequence;
         -- add the cred record 
         insert into cred(
                     cred_paym__sequence,
                     cred_invc__sequence,
                     cred_amount,
                     cred_gst_amount,
                     cred__sequence)
               values(
                     rec_crep.crep_paym__sequence,
                     rec_crep.crep_invc__sequence,
                     rec_crep.crep_cred_amount,
                     rec_crep.crep_cred_gst_amount,
                     tmp_cred__sequence);

    return tmp_cred__sequence;
    END;
$$
    LANGUAGE 'plpgsql';


