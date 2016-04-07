<?php
/**
 * @file
 * Administrative configuration form
 */

/**
 * Configuration form module.
 */
function geowidget_config_form($form, &$form_state) {
  $form = array();
  $default_value_lable = '';
  $default_value = variable_get('geowidget_default_city_id');
  if (!empty($default_value) && ($city = city_load($default_value))) {
    $default_value_lable = $city->title . ' (' . $city->id . ')';
  }
  $form['geowidget_default_city_id'] = array(
    '#title' => t('Default city'),
    '#required' => TRUE,
    '#type' => 'textfield',
    '#maxlength' => 128,
    '#default_value' => $default_value_lable,
    '#autocomplete_path' => 'geowidget/cities/autocomplete',
    '#element_validate' => array('_geowidget_config_form_geowidget_default_city_validate'),
  );

  return system_settings_form($form);
}

/**
 * Validator field geowidget_default_city in geowidget_config_form().
 *
 * @see geowidget_config_form()
 */
function _geowidget_config_form_geowidget_default_city_validate($element, &$form_state) {
  // If a value was entered into the autocomplete...
  $value = '';
  if (!empty($element['#value'])) {
    // Take "label (entity id)', match the id from parenthesis.
    if (preg_match("/.+\((\d+)\)/", $element['#value'], $matches)) {
      $value = $matches[1];
    }
  }
  // Update the value of this element so the field can validate the product IDs.
  form_set_value($element, $value, $form_state);
}