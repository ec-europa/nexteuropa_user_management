<?php

/**
 * @file
 * This file contains helper functions for the install process.
 */

/**
 * Create UM Administrator role.
 *
 * NEPT-2830: Add UM Administrator role.
 */
function _nexteuropa_user_management_um_administrator_role() {
  // Ensure translations don't break during installation.
  $t = get_t();

  $role = new stdClass();
  $role->name = NEXTEUROPA_USER_MANAGEMENT_UM_ADMINISTRATOR_ROLE;
  if (user_role_save($role) === FALSE) {
    drupal_set_message($t('%role role creation failed!', array(
      '%role' => NEXTEUROPA_USER_MANAGEMENT_UM_ADMINISTRATOR_ROLE,
    )), 'error');
    return;
  }

  variable_set('nexteuropa_user_management_um_administrator_rid', $role->rid);
  _nexteuropa_user_management_check_um_administrator_permissions();

  // In case of install the permission is still unknown by drupal, so we need to
  // add it manually.
  db_merge('role_permission')
    ->key(array(
      'rid' => $role->rid,
      'permission' => 'nexteuropa manage users purge cache',
      'module' => 'nexteuropa_user_management',
    ))
    ->execute();
  db_merge('role_permission')
    ->key(array(
      'rid' => $role->rid,
      'permission' => 'administer nexteuropa user management',
      'module' => 'nexteuropa_user_management',
    ))
    ->execute();
  db_merge('role_permission')
    ->key(array(
      'rid' => variable_get('nexteuropa_user_management_user_manager_rid'),
      'permission' => 'administer nexteuropa user management',
      'module' => 'nexteuropa_user_management',
    ))
    ->execute();
}
