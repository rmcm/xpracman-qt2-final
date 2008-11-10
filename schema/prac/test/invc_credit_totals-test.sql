--     $Id$
--
--   Copyright 2007 X=X Computer Software Trust
--                Kangaroo Ground Australia 3097
--
--
--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License published by
--  the Free Software Foundation; either version 2, or (at your option)
--  any later version.
--
--  This software is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this software; see the file COPYING.  If not, write to
--  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
--
--  Report problems and direct all questions to:
--
--      Rex McMaster, rmcm@compsoft.com.au
--
--


-- Unit test for invc_credit_totals trigger/function

-- Stop on error
\set ON_ERROR_STOP

-- invoices to use for testing
\set invc1 19992
\set invc2 19996

-- Protect everything in a transaction
BEGIN;

-- save the invoice records
select * into temp invc_test from invc;

-- Install the new methods
\i ../cred-triggers.sql

update cred set cred_invc__sequence = :invc2 where cred_invc__sequence = :invc1;

select * from invc except
       select * from invc_test;

select * from invc_test where invc__sequence in (:invc1, :invc2);
