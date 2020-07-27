NextEuropa User Management

# Change log
## V 1.2.0
- Add a `UM Administrator` role, with specific permissions.
- Add an email address field accessible to user management role.
- Allow to send an e-mail for new registrations to the email address added above
, if any.
- Add a `hook_um_administrator_grant_permissions` to grant new privileges to
the UM Administrator role.


## V 1.1.0
https://citnet.tech.ec.europa.eu/CITnet/jira/browse/NEPT-2853
- Users who can access the user management view (admin and user management
role) can view roles that are already assigned to users.
- Add a view to allow seeing for the selected users, the added (in green),
removed roles (in red) and resulting state (in blue).
- Do not allow adding and removing the same role at once.

## V 1.0.0
https://citnet.tech.ec.europa.eu/CITnet/jira/browse/NEPT-2830

- Create 'User management' role, to be granted to a 'user manager'. This person
can add or remove roles to/from users.
- Create a 'user management' page where users with 'user management role' can
administer roles.
- Create variables 'nexteuropa_user_management_banned_roles' and
'nexteuropa_user_management_banned_role_ids' to list roles that cannot be
granted to the users by the user manager (blacklist).

# Permissions restrictions
To ensure more secure websites, a series of permissions have been restricted
from usage in the platform.
## Forbidden permissions
The list of permissions forbidden are listed in the "Secure Drupal development
at the European Commission" report available on the OPENEU wiki :

-  'administer content types'
-  'administer ckeditor_lite'
-  'acccess all views'
-  'administer ecas'
-  'administer features'
-  'administer fields'
-  'administer file types'
-  'administer filters'
-  'administer jquery update'
-  'administer modules'
-  'administer om maximenu'
-  'administer page manager'
-  'administer permissions'
-  'administer site configuration'
-  'administer software updates'
-  'administer themes'
-  'administer users'
-  'administer views'
-  'bypass file access'
-  'bypass node access'
-  'bypass rules access'

These permissions are already forbidden in toolkit.
## Restricted permissions
The restricted permissions can only be added through code (see UM Administrator
role section below for more details)
The list of permission is held in the $do_not_give_permissions variable.
## Suggested permissions
The suggested permissions can be assigned to the UM Administrator role.

# Setting up
You will need to add into the settings.php one of the following to 'variable':
```php
$conf['nexteuropa_user_management_banned_roles'] = array();
$conf['nexteuropa_user_management_banned_role_ids'] = array();
```

`nexteuropa_user_management_banned_roles` includes the role names (string types)
which you want to exclude as a grantable role by User management users.
`nexteuropa_user_management_banned_role_ids` includes the role ids
(integer type except the two exception see below) which you want to exclude as a
grantable role by User management users. 

To exclude Administrator role use `<!!ADMIN_RID!!>` token in the
`nexteuropa_user_management_banned_role_ids` array.  
To exclude User management role use `<!!USER_MANAGER_RID!!>` token in the 
`nexteuropa_user_management_banned_role_ids` array.

To exclude as grantable role for User management user, use :
```php
$conf['nexteuropa_user_management_banned_role_ids'] = array(
  '<!!ADMIN_RID!!>',
  '<!!USER_MANAGER_RID!!>',
);
```

## Default values for setting variables
If `nexteuropa_user_management_banned_roles` remains undefined, it will be
considered as an empty array.  
If the `nexteuropa_user_management_banned_role_ids` remains undefined, it will
be considered as admin and user management was set.  
The two condition above are independent of eachother, by defining role names,
won't remove role ids' default values.

# Notification for new user registration
You can set up mail notifications for new (blocked) user registration. The
module will only send e-mail if the new registered user status is blocked.
By default, User managers can edit this setting at
`/admin/config/people/nexteuropa-user-management-settings`.
Users can set up e-mail address(es) to send to, the subject and message to be
sent.
If the token module is enabled it can be user to include data from the newly
registered user.
ie: by using the `[user]` token, it is possible to include information like
name to know who should they look for.
With the `[site:nexteuropa-user-management-page-url]` token you can make a link
to bring the user to the user management page.
See the token description under the field.

# UM Administrator role
## Scope
This role is relevant to content management, it is not a site administrator
role.
It cannot be granted by default any configuration related permission.

## Automatic permission grant and revoke
The module will automatically grant and revoke permissions from the UM
Administrator role if it's not defined properly. If you need permission (i.e.
custom) that's not granted yet, you can use the
`hook_um_administrator_grant_permissions` hook to define those.

### Grant and revoke process
It's done by cron. The following process is done:
1. Collect permissions from `hook_um_administrator_grant_permissions`
2. Remove do not give (configuration related) permissions from the collected
ones
3. Add all permissions that are granted for other roles, except Administrator
role, UM Administrator role and the roles that are defined in the
`nexteuropa_user_management_exclude_roles_from_perm_grant_common` or
`nexteuropa_user_management_exclude_roles_from_perm_grant_site` drupal variable
4. Add suggested permission, usually content permissions, like edit x content
type
5. Remove the forbidden permissions
6. Remove other permissions that are currently not defined by `hook_permission`
i.e. badly injected permissions or uninstalled modules that still have records
in the `role_permission` table for some reason.

If the above gives new permissions, then they will be granted. If the above one
doesn't contain some already existing one for this role, it will be revoked.
### Excluding the site administrator role(s)
If you have custom site administrator role that permissions shouldn't given to
the UM Administrator, then you need to define  
a) `nexteuropa_user_management_exclude_roles_from_perm_grant_common` drupal
variable for multisite 'level'  
b) `nexteuropa_user_management_exclude_roles_from_perm_grant_site` drupal
variable for site-specific 'level'.  
The default value for both an empty array, and it waits for an array with a list
of role names as value. If both defined the values will be merged.
## Manual permission grant and revoke (or sync)
If you can't wait until cron runs, you can call the
`_nexteuropa_user_management_check_um_administrator_permissions()` function in
`hook_update_n`. It's mandatory that the module providing the newly grantable
permissions should be enabled before the function runs and drupal should be
already take into account the module's `hook_permission`. If that is not
possible, you need to inject manually to the database, see this module's
`hook_install` and implement the custom hook provided by this module, to make
sure your permission won't be revoked at cron run.
