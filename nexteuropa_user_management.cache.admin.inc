<?php

/**
 * @file
 * Cache purge related admin page functions.
 */

/**
 * Page callback: Cache purges.
 *
 * @see nexteuropa_user_management_menu()
 */
function _nexteuropa_user_management_cache_form($form, &$form_state) {
  $caches = array(
    'assets' => t('CSS and JavaScript'),
    'cache' => t('Page and else'),
    'menu' => t('Menu'),
    'registry' => t('Class registry'),
    'theme' => t('Theme registry'),
    'update' => t('Update data'),
    'all_cache' => t('All Drupal caches'),
  );
  $caches_desc = array(
    'assets' => array(
      '#description' => t('Clears CSS and JavaScript caches, useful if aggregation is enabled.'),
    ),
    'cache' => array(
      '#description' => t('Clears page related cache and cache that provided by hook_flush_caches.'),
    ),
    'menu' => array(
      '#description' => t('Clears drupal menu cache.'),
    ),
    'registry' => array(
      '#description' => t('Clears registry cache.'),
    ),
    'theme' => array(
      '#description' => t('Clears theme cache.'),
    ),
    'update' => array(
      '#description' => t('Clears update cache.'),
    ),
    'all_cache' => array(
      '#description' => t('Clears all kind of caches that relates to Drupal core.'),
    ),
  );
  if (module_exists('admin_menu')) {
    $caches['admin_menu'] = t('Administration menu');
    $caches_desc['admin_menu'] = array(
      '#description' => t('Clears admin menu cache. (Black bar on top.)'),
    );
  }
  if (module_exists('nexteuropa_varnish') && !_nexteuropa_varnish_prevent_purge()) {
    // @TODO: maybe add content type based varnish purge too? It seems it's
    // supported by nexteuropa_varnish module.
    $caches['varnish'] = t('Varnish');
    $caches_desc['varnish'] = array(
      '#description' => t('Clears varnish cache only for every route. (Full varnish clear.)'),
    );
    $caches['varnish_drupal'] = t('Varnish and all Drupal cache');
    $caches_desc['admin_menu'] = array(
      '#description' => t('Clears varnish cache only for every route. (Full varnish clear.) + Clears all kind of caches that relates to Drupal core.'),
    );
  }

  $form['cache_type'] = array_merge(array(
    '#type' => 'radios',
    '#title' => t('Cache type'),
    '#default_value' => isset($form_state['values']['cache_type']) ? $form_state['values']['cache_type'] : 'all_cache',
    '#options' => $caches,
    '#description' => t('Choose which cache type should be purged.'),
  ), $caches_desc);

  $form_state['caches'] = $caches;

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Purge selected'),
  );

  return $form;
}

/**
 * Page callback's submit: Cache purges.
 *
 * @see _nexteuropa_user_management_cache_form()
 */
function _nexteuropa_user_management_cache_form_submit($form, &$form_state) {
  _nexteuropa_user_management_cache_purges($form_state['values']['cache_type']);
  drupal_set_message(t("The action on the '!cache' cache type was executed.", array(
    '!cache' => $form_state['caches'][$form_state['values']['cache_type']],
  )));
}

/**
 * Clears a specific type of cache.
 *
 * @param string $name
 *   Cache type.
 */
function _nexteuropa_user_management_cache_purges($name) {
  switch ($name) {
    case 'admin_menu':
      admin_menu_flush_caches();
      break;

    case 'menu':
      menu_rebuild();
      break;

    case 'registry':
      registry_rebuild();
      // Fall-through to clear cache tables, since registry information is
      // usually the base for other data that is cached (e.g. SimpleTests).
    case 'cache':
      // Don't clear cache_form - in-progress form submissions may break.
      // Ordered so clearing the page cache will always be the last action.
      // @see drupal_flush_all_caches()
      $core = array('cache', 'cache_bootstrap', 'cache_filter', 'cache_page');
      $cache_tables = array_merge(module_invoke_all('flush_caches'), $core);
      foreach ($cache_tables as $table) {
        cache_clear_all('*', $table, TRUE);
      }
      break;

    case 'all_cache':
      drupal_flush_all_caches();
      break;

    case 'assets':
      // Change query-strings on css/js files to enforce reload for all users.
      _drupal_flush_css_js();

      drupal_clear_css_cache();
      drupal_clear_js_cache();

      // Clear the page cache, since cached HTML pages might link to old CSS and
      // JS aggregates.
      cache_clear_all('*', 'cache_page', TRUE);
      break;

    case 'theme':
      system_rebuild_theme_data();
      drupal_theme_rebuild();
      break;

    case 'update':
      _update_cache_clear();
      break;

    case 'varnish_drupal':
      // First clear drupal caches, so varnish won't pick up from Drupal
      // outdated content.
      drupal_flush_all_caches();
      // Let it go through to clear varnish too.
    case 'varnish':
      if (!_nexteuropa_varnish_varnish_requests_send()) {
        drupal_set_message(t('The Varnish caches have not been purged correctly. Please consult logs for more information.'), 'error');
      }
      break;
  }
}
