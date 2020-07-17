NextEuropa User Management

# Setting up
You will need to add into the settings.php one of the following to 'variable':
```php
$conf['nexteuropa_user_management_banned_roles'] = array();
$conf['nexteuropa_user_management_banned_role_ids'] = array();
```

Inside the `nexteuropa_user_management_banned_roles` include the role names
(string type) which you want to exclude as a grantable role by User management
users.  
Inside the `nexteuropa_user_management_banned_role_ids` include the role ids
(integer type except the two exception see below) which you want to exclude as a
grantable role by User management users. 

To exclude Administrator role use `<!!ADMIN_RID!!>` token in the
`nexteuropa_user_management_banned_role_ids` array.  
To exclude User management role use `<!!USER_MANAGER_RID!!>` token in the 
`nexteuropa_user_management_banned_role_ids` array.

So to exclude as grantable role for User management user, put this into the
settings.php:
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
The two condition above are independent from eachother, by defining role names,
won't remove role ids' default values.

# Notification for new user registration
It's possible to set up e-mail notification if a new user registers into the
system. By default User managers can edit this setting on the
`/admin/config/people/nexteuropa-user-management-settings` page. Users can set
up e-mail address(es) the subject and message to be sent. If the token module is
enabled on the site they can include information from the newly registered user,
by using the `[user]` token, to include information like name to know who should
they look for. For token support view the description texts under the field.

# UM Administrator role
## Scope
This role is for content management and not site management, therefore it can't 
be grant by default any config related permission. In other words, it's a
content administrator role, not a site administrator.
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
The default value for both empty array and it waits for an array with a list of
role names as value. If both defined the values will be merged.
## Manual permission grant and revoke (or sync)
If you can't wait until cron runs, you can call the
`_nexteuropa_user_management_check_um_administrator_permissions()` function in
`hook_update_n`. It's mandatory that the module providing the newly grantable
permissions should be enabled before the function runs and drupal should be
already take into account the module's `hook_permission`. If that is not
possible, you need to inject manually to the database, see this module's
`hook_install` and implement the custom hook provided by this module, to make
sure your permission won't be revoked at cron run.
