<?php

/**
 * @file
 * City type editing UI.
 */

/**
 * UI controller.
 */
class EntityGeowidgetCityUIController extends EntityDefaultUIController {

  /**
   * Provides definitions for implementing hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[CITY_PATH . '/%city'] = array(
      'title callback' => 'node_page_title',
      'title arguments' => array(1),
      'page callback' => 'city_page_view',
      'page arguments' => array(1),
      'access arguments' => array('access content'),
    );
    $items[CITY_PATH . '/%city/view'] = array(
      'title' => 'View',
      'type' => MENU_DEFAULT_LOCAL_TASK,
      'weight' => -10,
    );
    $items[CITY_PATH . '/%city/edit'] = array(
      'title' => 'Edit',
      'page callback' => 'city_page_edit',
      'page arguments' => array(1),
      'access arguments' => array('administer users'),
      'weight' => 0,
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'city.admin.inc',
      'file path' => drupal_get_path('module', 'geowidget_city')
    );
    $items[CITY_PATH . '/%city/edit'] = array(
      'title' => 'Edit',
      'page callback' => 'city_page_edit',
      'page arguments' => array(1),
      'access arguments' => array('administer users'),
      'weight' => 0,
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'city.admin.inc',
      'file path' => drupal_get_path('module', 'geowidget_city')
    );
    $items[CITY_PATH . '/%city/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('city_page_delete_confirm_form', 1),
      'access arguments' => array('administer users'),
      'weight' => 4,
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'city.admin.inc',
      'file path' => drupal_get_path('module', 'geowidget_city')
    );
    return $items;
  }

  /**
   * Builds a structured array representing the entity's content.
   *
   * The content built for the entity will vary depending on the $view_mode
   * parameter.
   *
   * @param $entity
   *   An entity object.
   * @param $view_mode
   *   View mode, e.g. 'full', 'teaser'...
   * @param $langcode
   *   (optional) A language code to use for rendering. Defaults to the global
   *   content language of the current request.
   * @return
   *   The renderable array.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $build = parent::buildContent($entity, $view_mode, $langcode, $content);
    $build['#contextual_links']['city'] = array($entity->real_path(), array($entity->id));
    return $build;
  }

}

/**
 * Menu callback; presents the city editing form.
 */
function city_page_edit($city) {
  drupal_set_title(t('Edit @title', array('@title' => $city->title)), PASS_THROUGH);
  return entity_ui_get_form('city', $city);
}

/**
 * Form constructor for the city add/edit form.
 *
 * @see city_form_validate()
 * @see city_form_submit()
 * @see city_form_delete_submit()
 * @ingroup forms
 */
function city_form($form, &$form_state, $city, $op = 'edit') {
  $form['title'] = array(
    '#title' => 'City name',
    '#required' => true,
    '#type' => 'textfield',
    '#maxlength' => 200,
    '#default_value' => isset($city->title) ? $city->title : '',
  );
  $form['geoname_id'] = array(
    '#title' => 'Geoname ID',
    '#required' => true,
    '#type' => 'textfield',
    '#default_value' => isset($city->geoname_id) ? $city->geoname_id : '',
  );
  $parent_city_lable = '';
  if (!empty($city->parent_id)) {
    $parent_city = city_load($city->parent_id);
    $parent_city_lable = $parent_city->title . ' (' . $city->parent_id . ')';
  }
  $form['parent_id'] = array(
    '#title' => 'Parent city',
    '#type' => 'textfield',
    '#maxlength' => 128,
    '#default_value' => $parent_city_lable,
    '#autocomplete_path' => 'geowidget/cities/autocomplete',
    '#element_validate' => array('_city_form_parent_id_validate'),
  );

  field_attach_form('city', $city, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#limit_validation_errors' => array(),
    '#submit' => array('city_form_submit_delete')
  );
  return $form;
}

/**
 * Validator field parent_id in city_form().
 *
 * @see city_form()
 */
function _city_form_parent_id_validate($element, &$form_state, $form) {
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

/**
 * Form validation handler for city_form().
 *
 * @see city_form()
 * @see city_form_submit()
 */
function city_form_validate($form, &$form_state) {
  if ($form_state['values']['title'] == '') {
    form_set_error('title', t('You must set \'City name\''));
  }
  if ($form_state['values']['geoname_id'] == '') {
    form_set_error('geoname_id', t('You must set \'Geoname ID\''));
  } elseif (strcmp((int) $form_state['values']['geoname_id'], $form_state['values']['geoname_id'])) {
    form_set_error('geoname_id', t('You must set \'Geoname ID\' type integer'));
  }
  if($form_state['values']['parent_id']){
    if ((isset($form_state['city']->id)) && ($form_state['city']->id == $form_state['values']['parent_id'])) {
      form_set_error('parent_city', t('You must changing \'Parent city\', recursion'));
    } elseif (!city_load($form_state['values']['parent_id'])) {
      form_set_error('parent_city', t('You must changing \'Parent city\', parent city not exist'));
    }
  }
}

/**
 * Form submission handler for city_form().
 *
 * @see city_form()
 * @see city_form_validate()
 */
function city_form_submit(&$form, &$form_state) {
  $city = entity_ui_form_submit_build_entity($form, $form_state);
  $city->save();
  if ($city->id) {
    //$form_state['redirect'] = node_access('view', $city) ? 'node/' . $city->nid : '<front>';
    $form_state['redirect'] = 'city/' . $city->id;
  } else {
    // In the unlikely case something went wrong on save, the city will be
    // rebuilt and city form redisplayed the same way as in preview.
    drupal_set_message(t('The post could not be saved.'), 'error');
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Handler for the delete button on form city.
 *
 * @see city_form()
 */
function city_form_submit_delete($form, &$form_state) {
  $form_state['redirect'] = CITY_PATH . '/' . $form_state['city']->id . '/delete';
}

/**
 * Confirm form for deleting a City.
 */
function city_page_delete_confirm_form($form, &$form_state, $city) {
  $form_state += array('city' => $city);
  $confirm_question = t('Are you sure you want to delete city %label?', array('%label' => $city->title));
  return confirm_form($form, $confirm_question, CITY_PATH . '/' . $city->id . '/delete');
}

/**
 * Form submission handler for city_page_delete_confirm_form().
 * Handles the 'Delete' button on the city_form().
 *
 * @see city_page_delete_confirm_form()
 * @see city_form()
 */
function city_page_delete_confirm_form_submit($form, &$form_state) {
  $city = $form_state['city'];
  $city->delete();
  drupal_set_message(t('Deleted %label.', array('%label' => $city->title)));
  $form_state['redirect'] = ADMIN_CITY_PATH;
}
